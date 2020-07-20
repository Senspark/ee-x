package com.ee

import android.app.Activity
import android.app.Application.ActivityLifecycleCallbacks
import android.content.Context
import android.os.Bundle
import androidx.annotation.AnyThread
import com.ee.internal.MessageBridge
import com.ee.internal.NativeThread
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

    private val _bridge: IMessageBridge = MessageBridge.getInstance()
    private val _plugins: MutableMap<String, IPlugin> = ConcurrentHashMap()
    private val _lifecycleCallbacks: ActivityLifecycleCallbacks

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
    internal fun initializePlugins(): Boolean {
        val context = _context
        if (context == null) {
            _logger.error("Please set activity via PluginManager.getInstance().setActivity")
            assertThat(false).isTrue()
            return false
        }
        Platform.registerHandlers(_bridge, context)
        return true
    }

    @AnyThread
    internal fun addPlugin(name: String): Boolean {
        _logger.info("addPlugin: $name")
        if (_plugins.containsKey(name)) {
            _logger.error("addPlugin: $name already exists!")
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
    internal fun removePlugin(name: String): Boolean {
        _logger.info("removePlugin: $name")
        val plugin = _plugins[name]
        if (plugin == null) {
            _logger.error("removePlugin: $name doesn't exist!")
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
        Platform.deregisterHandlers(_bridge)
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
    return PluginManager.getInstance().initializePlugins()
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

/// Legacy method used by Soomla.
fun ee_getStorePlugin(): IPlugin? {
    return PluginManager.getInstance().getPlugin("Store")
}