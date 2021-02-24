package com.ee

import android.app.Activity
import android.app.Application
import android.app.Application.ActivityLifecycleCallbacks
import android.os.Bundle
import androidx.annotation.AnyThread
import com.ee.internal.MessageBridge
import com.ee.internal.MessageBridgeHandler
import com.ee.internal.NativeThread
import com.ee.internal.ThreadImpl
import kotlinx.serialization.InternalSerializationApi
import java.util.concurrent.ConcurrentHashMap

/**
 * Created by Zinge on 6/5/16.
 */
class PluginManager private constructor() {
    companion object {
        private val kTag = PluginManager::class.java.name
        private val _sharedInstance = PluginManager()

        @JvmStatic
        val instance: PluginManager
            @AnyThread get() = _sharedInstance
    }

    private interface PluginExecutor {
        fun execute(plugin: IPlugin)
    }

    private val _logger = Logger("ee-x")
    private val _plugins: MutableMap<String, IPlugin> = ConcurrentHashMap()
    private val _lifecycleCallbacks: ActivityLifecycleCallbacks

    private var _bridge: IMessageBridge? = null
    private var _application: Application? = null
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
                            _logger.warn("$kTag: onActivityCreated: is not root activity")
                        }
                    } else {
                        _logger.warn("$kTag: onActivityCreated: invalid activity")
                    }
                }
            }

            override fun onActivityStarted(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onStart)
                    } else {
                        _logger.warn("$kTag: onActivityStarted: invalid activity")
                    }
                }
            }

            override fun onActivityResumed(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onResume)
                    } else {
                        _logger.warn("$kTag: onActivityResumed: invalid activity")
                    }
                }
            }

            override fun onActivityPaused(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onPause)
                    } else {
                        _logger.warn("$kTag: onActivityPaused: invalid activity")
                    }
                }
            }

            override fun onActivityStopped(activity: Activity) {
                if (activity::class.java == _activityClass) {
                    if (_activity === activity) {
                        executePlugins(IPlugin::onStop)
                    } else {
                        _logger.warn("$kTag: onActivityStopped: invalid activity")
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
                        _logger.warn("$kTag: onActivityDestroyed: invalid activity")
                    }
                }
            }
        }
    }

    val application: Application?
        @AnyThread get() = _application

    var activity: Activity?
        @AnyThread get() = _activity
        @AnyThread set(value) {
            val activity = value ?: throw IllegalArgumentException("Null activity")
            val application = activity.application
            _application = application
            _activity = activity
            _activityClass = activity::class.java
            application.registerActivityLifecycleCallbacks(_lifecycleCallbacks)
        }

    val bridge: IMessageBridge?
        @AnyThread get() = _bridge

    val logger: ILogger
        @AnyThread get() = _logger

    @AnyThread
    @InternalSerializationApi
    fun initializePlugins(version: String, messageHandler: MessageBridgeHandler) {
        val expectedVersion = "2.5.1"
        if (version != expectedVersion) {
            throw IllegalStateException("Version mismatched: found $version expected $expectedVersion")
        }
        _logger.info("Initialize ee-x plugin version $version")
        val application = _application
            ?: throw IllegalStateException("""
                    $kTag: Please set activity via
                    PluginManager.getInstance().setActivity() (kotlin) or
                    ee::PluginManager::setActivity() (c++)
                """.trimIndent())
        val bridge = MessageBridge(_logger, messageHandler)
        _bridge = bridge
        Thread.registerImpl(ThreadImpl(_logger))
        Platform.registerHandlers(bridge, application)
    }

    @AnyThread
    fun setLogLevel(level: Int) {
        _logger.logLevel = level
    }

    @AnyThread
    fun addPlugin(name: String) {
        _logger.info("$kTag: ${this::addPlugin.name}: $name")
        if (_plugins.containsKey(name)) {
            throw IllegalStateException("$kTag: ${this::addPlugin.name}: $name already exists!")
        }
        val className = "com.ee.${name}Bridge"
        val clazz = Class.forName(className)
        val constructor = clazz.getConstructor(
            IMessageBridge::class.java,
            ILogger::class.java,
            Application::class.java,
            Activity::class.java)
        val plugin = constructor.newInstance(_bridge, _logger, _application, _activity)
        _plugins[name] = plugin as IPlugin
    }

    @AnyThread
    fun removePlugin(name: String) {
        _logger.info("$kTag: ${this::removePlugin.name}: $name")
        val plugin = _plugins[name]
            ?: throw IllegalStateException("$kTag: ${this::removePlugin.name}: $name doesn't exist!")
        plugin.destroy()
        _plugins.remove(name)
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
        _application = null
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

@Suppress("FunctionName")
private external fun ee_callCppInternal(tag: String, message: String)

@InternalSerializationApi
@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticInitializePlugins(version: String) {
    PluginManager.instance.initializePlugins(version) { tag, message ->
        ee_callCppInternal(tag, message)
    }
}

@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticSetLogLevel(level: Int) {
    PluginManager.instance.setLogLevel(level)
}

@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticAddPlugin(name: String) {
    PluginManager.instance.addPlugin(name)
}

@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticRemovePlugin(name: String) {
    PluginManager.instance.removePlugin(name)
}

@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticGetActivity(): Any? {
    return PluginManager.instance.activity
}

@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticSetActivity(activity: Any) {
    val item = activity as? Activity ?: throw IllegalArgumentException("Invalid activity")
    PluginManager.instance.activity = item
}

/// Legacy method used by Soomla.
@Suppress("FunctionName")
fun ee_getStorePlugin(): IPlugin? {
    return PluginManager.instance.getPlugin("Store")
}