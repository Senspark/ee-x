package com.ee.core

import android.app.Activity
import android.app.Application.ActivityLifecycleCallbacks
import android.content.Context
import android.os.Bundle
import androidx.annotation.AnyThread
import com.ee.core.internal.Platform
import com.ee.core.internal.Utils
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import java.lang.reflect.InvocationTargetException

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
    private val _plugins: MutableMap<String, IPlugin>
    private val _classes: MutableMap<String, String>
    private val _lifecycleCallbacks: ActivityLifecycleCallbacks

    private var _context: Context? = null
    private var _activity: Activity? = null
    private var _activityClass: Class<out Activity>? = null

    init {
        _plugins = HashMap()
        _classes = HashMap()
        _classes["AdMob"] = "com.ee.admob.AdMob"
        _classes["AppLovin"] = "com.ee.applovin.AppLovin"
        _classes["AppsFlyer"] = "com.ee.appsflyer.AppsFlyer"
        _classes["CampaignReceiver"] = "com.ee.campaignreceiver.CampaignReceiver"
        _classes["Facebook"] = "com.ee.facebook.Facebook"
        _classes["FacebookAds"] = "com.ee.facebook.FacebookAds"
        _classes["FirebaseCore"] = "com.ee.firebase.core.FirebaseCore"
        _classes["FirebaseCrashlytics"] = "com.ee.firebase.crashlytics.FirebaseCrashlytics"
        _classes["FirebasePerformance"] = "com.ee.firebase.performance.FirebasePerformance"
        _classes["GoogleAnalytics"] = "com.ee.google.analytics.GoogleAnalytics"
        _classes["IronSource"] = "com.ee.ironsource.IronSource"
        _classes["Notification"] = "com.ee.notification.Notification"
        _classes["Play"] = "com.ee.play.Play"
        _classes["Recorder"] = "com.ee.recorder.Recorder"
        _classes["Store"] = "com.ee.store.Store"
        _classes["Tenjin"] = "com.ee.tenjin.Tenjin"
        _classes["UnityAds"] = "com.ee.unityads.UnityAds"
        _classes["Vungle"] = "com.ee.vungle.Vungle"

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
    @ImplicitReflectionSerializer
    @UnstableDefault
    internal fun initializePlugins(activity: Activity): Boolean {
        val context = activity.applicationContext
        if (context == null) {
            assertThat(false).isTrue()
            return false
        }
        _context = context
        _activityClass = activity::class.java
        activity.application.registerActivityLifecycleCallbacks(_lifecycleCallbacks)
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
        if (!_classes.containsKey(name)) {
            _logger.error("addPlugin: $name classes doesn't exist!")
            return false
        }
        val className = _classes[name] as String
        try {
            val clazz = Class.forName(className)
            val constructor = clazz.getConstructor(Context::class.java, IMessageBridge::class.java)
            val plugin = constructor.newInstance(_context, _bridge)
            _plugins[name] = plugin as IPlugin
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
        return true
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
@Suppress("unused")
@UnstableDefault
private fun ee_staticInitializePlugins(): Boolean {
    val activity = Utils.getCurrentActivity()
    if (activity == null) {
        assertThat(false).isTrue()
        return false
    }
    return PluginManager.getInstance().initializePlugins(activity)
}

@Suppress("unused")
private fun ee_staticAddPlugin(name: String): Boolean {
    return PluginManager.getInstance().addPlugin(name)
}

@Suppress("unused")
private fun ee_staticRemovePlugin(name: String): Boolean {
    return PluginManager.getInstance().removePlugin(name)
}

/// Legacy method used by Soomla.
fun ee_getStorePlugin(): IPlugin? {
    return PluginManager.getInstance().getPlugin("Store")
}