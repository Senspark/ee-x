package com.ee

import android.app.Activity
import android.app.Application.ActivityLifecycleCallbacks
import android.content.Context
import android.os.Bundle
import androidx.annotation.AnyThread
import com.ee.internal.MessageBridge
import com.ee.internal.MessageBridgeHandler
import com.ee.internal.NativeThread
import com.ee.internal.ee_callCppInternal
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import java.lang.reflect.InvocationTargetException
import java.util.concurrent.ConcurrentHashMap

/**
 * Created by Zinge on 6/5/16.
 */
class PluginManager private constructor() {
    companion object {
        private val _logger = Logger(PluginManager::class.java.name)

        private val _sharedInstance = PluginManager()

        @JvmStatic
        fun getInstance(): PluginManager {
            return _sharedInstance
        }
    }

    private interface PluginExecutor {
        fun execute(plugin: IPlugin)
    }

    private val _plugins: MutableMap<String, IPlugin> = ConcurrentHashMap()
    private val _lifecycleCallbacks: ActivityLifecycleCallbacks

    private var _bridge: IMessageBridge? = null
    private var _context: Context? = null
    private var _activity: Activity? = null
    private var _activityClass: Class<out Activity>? = null

    init {
        _lifecycleCallbacks = object : ActivityLifecycleCallbacks {
            override fun onActivityCreated(activity: Activity,
                                           savedInstanceState: Bundle?) {
                if (activity::class.java == _activityClass) {
                    if (_activity == null) {
                        if (activity.isTaskRoot) {
                            _activity = activity
                            executePlugins { plugin -> plugin.onCreate(activity) }
                        } else {
                            _logger.warn("onActivityCreated: is not root activity")
                        }
                    } else {
                        _logger.warn("onActivityCreated: invalid activity")
                    }
                }
            }

            override fun onActivityStarted(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onStart)
                    } else {
                        _logger.warn("onActivityStarted: invalid activity")
                    }
                }
            }

            override fun onActivityResumed(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onResume)
                    } else {
                        _logger.warn("onActivityResumed: invalid activity")
                    }
                }
            }

            override fun onActivityPaused(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onPause)
                    } else {
                        _logger.warn("onActivityPaused: invalid activity")
                    }
                }
            }

            override fun onActivityStopped(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onStop)
                    } else {
                        _logger.warn("onActivityStopped: invalid activity")
                    }
                }
            }

            override fun onActivitySaveInstanceState(activity: Activity,
                                                     outState: Bundle) {
            }

            override fun onActivityDestroyed(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        _activity = null
                        executePlugins(IPlugin::onDestroy)
                    } else {
                        _logger.warn("onActivityDestroyed: invalid activity")
                    }
                }
            }
        }
    }

    @AnyThread
    fun getContext(): Context? {
        return _context
    }

    @AnyThread
    fun getActivity(): Activity? {
        return _activity
    }

    @AnyThread
    fun getBridge(): IMessageBridge? {
        return _bridge
    }

    @AnyThread
    fun setActivity(activity: Activity) {
        val context = activity.applicationContext
        _context = context
        _activity = activity
        _activityClass = activity::class.java
        activity.application.registerActivityLifecycleCallbacks(_lifecycleCallbacks)
    }

    @AnyThread
    @ImplicitReflectionSerializer
    @UnstableDefault
    fun initializePlugins(bridge: IMessageBridge): Boolean {
        val context = _context
        if (context == null) {
            _logger.error("""
                Please set activity via
                PluginManager.getInstance().setActivity() (kotlin) or
                ee::PluginManager::setActivity() (c++)
            """.trimIndent())
            assertThat(false).isTrue()
            return false
        }
        _bridge = bridge
        Platform.registerHandlers(bridge, context)
        return true
    }

    @AnyThread
    fun addPlugin(name: String): Boolean {
        _logger.info("${this::addPlugin.name}: $name")
        if (_plugins.containsKey(name)) {
            _logger.error("${this::addPlugin.name}: $name already exists!")
            return false
        }
        val className = "com.ee.${name}Bridge"
        try {
            val clazz = Class.forName(className)
            val constructor = clazz.getConstructor(
                IMessageBridge::class.java,
                Context::class.java,
                Activity::class.java)
            val plugin = constructor.newInstance(_bridge, _context, _activity)
            _plugins[name] = plugin as IPlugin
            return true
        } catch (ex: ClassNotFoundException) {
            ex.printStackTrace()
        } catch (ex: NoSuchMethodException) {
            ex.printStackTrace()
        } catch (ex: IllegalAccessException) {
            ex.printStackTrace()
        } catch (ex: InstantiationException) {
            ex.printStackTrace()
        } catch (ex: InvocationTargetException) {
            ex.printStackTrace()
        }
        return false
    }

    @AnyThread
    fun removePlugin(name: String): Boolean {
        _logger.info("${this::removePlugin.name}: $name")
        val plugin = _plugins[name]
        if (plugin == null) {
            _logger.error("${this::removePlugin.name}: $name doesn't exist!")
            return false
        }
        plugin.destroy()
        _plugins.remove(name)
        return true
    }

    @AnyThread
    internal fun getPlugin(name: String): IPlugin? {
        return _plugins[name]
    }

    @AnyThread
    private fun destroy() {
        val bridge = _bridge
            ?: throw IllegalArgumentException("${this::destroy.name}: bridge is null")
        Platform.deregisterHandlers(bridge)
        for (entry in _plugins) {
            entry.value.destroy()
        }
        _context = null
    }

    private fun executePlugins(executor: PluginExecutor) {
        for (entry in _plugins) {
            executor.execute(entry.value)
        }
    }

    private fun executePlugins(executor: (plugin: IPlugin) -> Unit) {
        executePlugins(object : PluginExecutor {
            override fun execute(plugin: IPlugin) {
                executor(plugin)
            }
        })
    }
}

@ImplicitReflectionSerializer
@NativeThread
@Suppress("unused")
@UnstableDefault
private fun ee_staticInitializePlugins(): Boolean {
    /* Has defect.
    val activity = Utils.getCurrentActivity()
    if (activity == null) {
        assertThat(false).isTrue()
        return false
    }
     */
    val bridge = MessageBridge(object : MessageBridgeHandler {
        override fun callCpp(tag: String, message: String): String {
            return ee_callCppInternal(tag, message)
        }
    })
    return PluginManager.getInstance().initializePlugins(bridge)
}

@NativeThread
@Suppress("unused")
private fun ee_staticAddPlugin(name: String): Boolean {
    return PluginManager.getInstance().addPlugin(name)
}

@NativeThread
@Suppress("unused")
private fun ee_staticRemovePlugin(name: String): Boolean {
    return PluginManager.getInstance().removePlugin(name)
}

@NativeThread
@Suppress("unused")
private fun ee_staticGetActivity(): Any? {
    return PluginManager.getInstance().getActivity()
}

@NativeThread
@Suppress("unused")
private fun ee_staticSetActivity(activity: Any) {
    val item = activity as? Activity ?: throw IllegalArgumentException("Invalid activity")
    PluginManager.getInstance().setActivity(item)
}

/// Legacy method used by Soomla.
fun ee_getStorePlugin(): IPlugin? {
    return PluginManager.getInstance().getPlugin("Store")
}