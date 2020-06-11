package com.ee.core.internal

import android.annotation.SuppressLint
import android.app.Activity
import android.content.ActivityNotFoundException
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.content.res.Resources
import android.net.ConnectivityManager
import android.net.Uri
import android.os.Build
import android.os.Handler
import android.os.Looper
import android.util.DisplayMetrics
import android.view.Surface
import android.view.View
import android.widget.FrameLayout
import com.ee.core.*
import com.google.android.gms.ads.identifier.AdvertisingIdClient
import com.google.android.gms.common.wrappers.InstantApps
import com.google.common.truth.Truth.assertThat
import io.reactivex.rxjava3.core.Single
import io.reactivex.rxjava3.schedulers.Schedulers
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault
import java.lang.reflect.InvocationTargetException
import java.net.InetAddress
import java.net.UnknownHostException
import java.security.MessageDigest
import java.util.concurrent.TimeUnit

/**
 * Created by Zinge on 10/9/17.
 */
object Utils {
    private const val INSTALL_APP_REQUEST_CODE = 0x10
    private val _logger = Logger(Utils::class.java.name)
    private const val kPrefix = "Utils_"
    private const val kIsMainThread = kPrefix + "isMainThread"
    private const val kRunOnUiThread = kPrefix + "runOnUiThread"
    private const val kRunOnUiThreadDelayed = kPrefix + "runOnUiThreadDelayed"
    private const val kRunOnUiThreadCallback = kPrefix + "runOnUiThreadCallback"
    private const val kIsApplicationInstalled = kPrefix + "isApplicationInstalled"
    private const val kOpenApplication = kPrefix + "openApplication"
    private const val kGetApplicationId = kPrefix + "getApplicationId"
    private const val kGetApplicationName = kPrefix + "getApplicationName"
    private const val kGetVersionName = kPrefix + "getVersionName"
    private const val kGetVersionCode = kPrefix + "getVersionCode"
    private const val kGetApplicationSignatures = kPrefix + "getApplicationSignatures"
    private const val kIsInstantApp = kPrefix + "isInstantApp"
    private const val kIsTablet = kPrefix + "isTablet"
    private const val kGetDensity = kPrefix + "getDensity"
    private const val kGetDeviceId = kPrefix + "getDeviceId"
    private const val kGetSafeInset = kPrefix + "getSafeInset"
    private const val kSendMail = kPrefix + "sendMail"
    private const val kTestConnection = kPrefix + "testConnection"
    private const val kShowInstallPrompt = kPrefix + "showInstallPrompt"

    @JvmStatic
    fun getRootView(activity: Activity): FrameLayout {
        return activity.findViewById<View>(android.R.id.content).rootView as FrameLayout
    }

    @JvmStatic
    fun toString(value: Boolean): String {
        return if (value) "true" else "false"
    }

    @JvmStatic
    fun toBoolean(value: String): Boolean {
        assertThat(value).isAnyOf("true", "false")
        return "true" == value
    }

    /// https://stackoverflow.com/questions/1069066/get-current-stack-trace-in-java
    private val currentStackTrace: Array<StackTraceElement>
        get() = Thread.currentThread().stackTrace

    @ImplicitReflectionSerializer
    @UnstableDefault
    fun registerHandlers(bridge: IMessageBridge, context: Context) {
        bridge.registerHandler(kIsMainThread) {
            toString(isMainThread())
        }
        bridge.registerHandler(kRunOnUiThread) {
            toString(runOnUiThread(Runnable {
                bridge.callCpp(kRunOnUiThreadCallback)
            }))
        }
        bridge.registerAsyncHandler(kRunOnUiThreadDelayed) { message, resolver ->
            @Serializable
            class Request(
                val delay: Float
            )

            val request = deserialize<Request>(message)
            runOnUiThreadDelayed(request.delay, Runnable {
                resolver.resolve("")
            })
        }
        bridge.registerHandler(kIsApplicationInstalled) { message ->
            toString(isApplicationInstalled(context, message))
        }
        bridge.registerHandler(kOpenApplication) { message ->
            toString(openApplication(context, message))
        }
        bridge.registerHandler(kGetApplicationId) { getApplicationId(context) }
        bridge.registerHandler(kGetApplicationName) { getApplicationName(context) }
        bridge.registerHandler(kGetVersionName) { getVersionName(context) }
        bridge.registerHandler(kGetVersionCode) { getVersionCode(context) }
        bridge.registerHandler(kGetApplicationSignatures) { message ->
            @Serializable
            class Request(
                val algorithm: String
            )

            @Serializable
            class Response(
                val signatures: List<String>
            )

            val request = deserialize<Request>(message)
            val signatures = getApplicationSignatures(context, request.algorithm)
            val response = Response(signatures)
            response.serialize()
        }
        bridge.registerHandler(kIsInstantApp) { toString(isInstantApp(context)) }
        bridge.registerHandler(kIsTablet) { toString(isTablet()) }
        bridge.registerHandler(kGetDensity) { getDensity().toString() }
        bridge.registerAsyncHandler(kGetDeviceId) { _, resolver ->
            getDeviceId(context)
                .subscribe(
                    { message -> resolver.resolve(message) },
                    { resolver.resolve("") })
        }
        bridge.registerHandler(kGetSafeInset) {
            @Serializable
            class Response(
                val left: Int,
                val right: Int,
                val top: Int,
                val bottom: Int
            )

            val activity = PluginManager.getInstance().getActivity()
            if (activity == null) {
                assertThat(false).isTrue()
                return@registerHandler ""
            }
            val inset = getSafeInset(activity)
            val response = Response(
                inset.left,
                inset.right,
                inset.top,
                inset.bottom
            )
            response.serialize()
        }
        bridge.registerHandler(kSendMail) { message: String ->
            @Serializable
            class Request(
                val recipient: String,
                val subject: String,
                val body: String
            )

            val request = deserialize<Request>(message)
            toString(sendMail(context, request.recipient, request.subject, request.body))
        }
        bridge.registerAsyncHandler(kTestConnection) { message, resolver ->
            @Serializable
            class Request(
                val host_name: String,
                val time_out: Float
            )

            val request = deserialize<Request>(message)
            testConnection(context, request.host_name, request.time_out)
                .subscribe(
                    { result -> resolver.resolve(toString(result)) },
                    { resolver.resolve(toString(false)) })
        }
        bridge.registerHandler(kShowInstallPrompt) { message ->
            @Serializable
            class Request(
                val url: String,
                val referrer: String
            )

            val request = deserialize<Request>(message)
            val activity = PluginManager.getInstance().getActivity()
            if (activity == null) {
                assertThat(false).isTrue()
                return@registerHandler ""
            }
            assertThat(activity).isNotNull()
            showInstantPrompt(request.url, request.referrer, activity)
            ""
        }
    }

    @JvmStatic
    fun deregisterHandlers(bridge: IMessageBridge) {
        bridge.deregisterHandler(kIsMainThread)
        bridge.deregisterHandler(kRunOnUiThread)
        bridge.deregisterHandler(kRunOnUiThreadDelayed)
        bridge.deregisterHandler(kIsApplicationInstalled)
        bridge.deregisterHandler(kOpenApplication)
        bridge.deregisterHandler(kGetApplicationId)
        bridge.deregisterHandler(kGetVersionName)
        bridge.deregisterHandler(kGetVersionCode)
        bridge.deregisterHandler(kGetApplicationSignatures)
        bridge.deregisterHandler(kIsInstantApp)
        bridge.deregisterHandler(kIsTablet)
        bridge.deregisterHandler(kGetDensity)
        bridge.deregisterHandler(kGetDeviceId)
        bridge.deregisterHandler(kGetSafeInset)
        bridge.deregisterHandler(kSendMail)
        bridge.deregisterHandler(kTestConnection)
        bridge.deregisterHandler(kShowInstallPrompt)
    }

    @JvmStatic
    fun checkMainThread() {
        if (!isMainThread()) {
            _logger.error("Current thread is not the main thread")
            for (e in currentStackTrace) {
                _logger.warn(e.toString())
            }
            assertThat(false).isTrue()
        }
    }

    fun isMainThread(): Boolean {
        return Thread.currentThread() === Looper.getMainLooper().thread
    }

    fun runOnUiThread(callback: Runnable): Boolean {
        if (isMainThread()) {
            callback.run()
            return true
        }
        val handler = Handler(Looper.getMainLooper())
        val result = handler.post(callback)
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable")
        }
        return false
    }

    @JvmStatic
    fun runOnUiThreadDelayed(seconds: Float, callback: Runnable) {
        val handler = Handler(Looper.getMainLooper())
        val result = handler.postDelayed(callback, seconds.toLong() * 1000)
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable")
        }
    }

    fun isApplicationInstalled(context: Context,
                               applicationId: String): Boolean {
        val packetManager = context.packageManager
        var installed = false
        try {
            packetManager.getPackageInfo(applicationId, PackageManager.GET_ACTIVITIES)
            installed = true
        } catch (ignored: PackageManager.NameNotFoundException) {
        }
        return installed
    }

    private fun openApplication(context: Context, applicationId: String): Boolean {
        val packetManager = context.packageManager
        val launchIntent = packetManager.getLaunchIntentForPackage(applicationId)
        if (launchIntent != null) {
            // Null pointer check in case package name was not found.
            context.startActivity(launchIntent)
            return true
        }
        return false
    }

    private fun getApplicationId(context: Context): String {
        return context.packageName
    }

    private fun getApplicationName(context: Context): String {
        val pm = context.packageManager
        return try {
            val ai = pm.getApplicationInfo(context.packageName, 0)
            pm.getApplicationLabel(ai) as String
        } catch (e: PackageManager.NameNotFoundException) {
            ""
        }
    }

    private fun getVersionName(context: Context): String {
        val packetManager = context.packageManager
        var versionName = ""
        try {
            val info = packetManager.getPackageInfo(context.packageName, 0)
            versionName = info.versionName
        } catch (e: PackageManager.NameNotFoundException) {
            e.printStackTrace()
        }
        return versionName
    }

    fun getVersionCode(context: Context): String {
        val packetManager = context.packageManager
        var versionCode = ""
        try {
            val info = packetManager.getPackageInfo(context.packageName, 0)
            versionCode = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                info.longVersionCode.toString()
            } else {
                info.versionCode.toString()
            }
        } catch (e: PackageManager.NameNotFoundException) {
            e.printStackTrace()
        }
        return versionCode
    }

    // https://stackoverflow.com/questions/52041805/how-to-use-packageinfo-get-signing-certificates-in-api-28/53407183
    private fun getApplicationSignatures(context: Context, algorithm: String): List<String> {
        var signatures = emptyList<String>()
        try {
            val digest = MessageDigest.getInstance(algorithm)
            val packetManager = context.packageManager
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                val flags = PackageManager.GET_SIGNING_CERTIFICATES
                val info = packetManager.getPackageInfo(context.packageName, flags)
                signatures = if (info.signingInfo.hasMultipleSigners()) {
                    info.signingInfo.apkContentsSigners.map {
                        toHexString(digest.digest(it.toByteArray()))
                    }
                } else {
                    info.signingInfo.signingCertificateHistory.map {
                        toHexString(digest.digest(it.toByteArray()))
                    }
                }
            } else {
                val flags = PackageManager.GET_SIGNATURES
                val info = packetManager.getPackageInfo(context.packageName, flags)
                signatures = info.signatures.map {
                    toHexString(digest.digest(it.toByteArray()))
                }
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return signatures
    }

    private val HEX_ARRAY = "0123456789ABCDEF".toCharArray()

    private fun toHexString(bytes: ByteArray): String {
        val chars = CharArray(bytes.size * 2)
        for (i in bytes.indices) {
            val v = bytes[i].toInt() and 0xFF
            chars[i * 2] = HEX_ARRAY[v shr 4]
            chars[i * 2 + 1] = HEX_ARRAY[v and 0x0F]
        }
        return String(chars)
    }

    private fun isInstantApp(context: Context): Boolean {
        return InstantApps.isInstantApp(context)
    }

    private fun isTablet(): Boolean {
        // Verifies if the Generalized Size of the device is XLARGE to be
        // considered a Tablet
        val screenLayout = Resources.getSystem().configuration.screenLayout
        val xlarge = screenLayout and Configuration.SCREENLAYOUT_SIZE_MASK >=
            Configuration.SCREENLAYOUT_SIZE_LARGE

        // If XLarge, checks if the Generalized Density is at least MDPI
        // (160dpi)
        if (xlarge) {
            val densityDpi = getDensityDpi()

            // MDPI=160, DEFAULT=160, DENSITY_HIGH=240, DENSITY_MEDIUM=160,
            // DENSITY_TV=213, DENSITY_XHIGH=320
            if (densityDpi == DisplayMetrics.DENSITY_DEFAULT ||
                densityDpi == DisplayMetrics.DENSITY_HIGH ||
                densityDpi == DisplayMetrics.DENSITY_MEDIUM ||
                densityDpi == DisplayMetrics.DENSITY_TV ||
                densityDpi == DisplayMetrics.DENSITY_XHIGH) {
                // Yes, this is a tablet!
                return true
            }
        }

        // No, this is not a tablet!
        return false
    }

    private fun getDensity(): Double {
        val metrics = Resources.getSystem().displayMetrics
        return metrics.density.toDouble()
    }

    private fun getDensityDpi(): Int {
        val metrics = Resources.getSystem().displayMetrics
        return metrics.densityDpi
    }

    /// https://stackoverflow.com/questions/4605527/converting-pixels-to-dp
    @JvmStatic
    fun convertDpToPixel(dp: Double): Double {
        return dp * getDensity()
    }

    private fun getDeviceId(context: Context): Single<String> {
        return Single
            .fromCallable {
                val info = AdvertisingIdClient
                    .getAdvertisingIdInfo(context.applicationContext)
                if (info.isLimitAdTrackingEnabled) {
                    ""
                } else {
                    info.id
                }
            }
    }

    class SafeInset {
        var left = 0
        var right = 0
        var top = 0
        var bottom = 0
    }

    private fun calculateSafeInset(activity: Activity, notchSize: Int, inset: SafeInset) {
        // https://stackoverflow.com/questions/3663665/how-can-i-get-the-current-screen-orientation
        when (activity.windowManager.defaultDisplay.rotation) {
            Surface.ROTATION_0 -> inset.top = notchSize
            Surface.ROTATION_90 -> inset.left = notchSize
            Surface.ROTATION_180 -> inset.bottom = notchSize
            Surface.ROTATION_270 -> inset.right = notchSize
        }
    }

    @Throws(IllegalAccessException::class, InvocationTargetException::class, NoSuchMethodException::class)
    private fun getSafeInset_Huawei(activity: Activity, inset: SafeInset): Boolean {
        val class_HwNotchSizeUtil: Class<*>
        try {
            val classLoader = activity.classLoader
            class_HwNotchSizeUtil = classLoader.loadClass("com.huawei.android.util.HwNotchSizeUtil")
            val method_hasNotchInScreen = class_HwNotchSizeUtil.getMethod("hasNotchInScreen")
            val hasFeature = method_hasNotchInScreen.invoke(class_HwNotchSizeUtil) as Boolean
            if (!hasFeature) {
                return false
            }
        } catch (e: Exception) {
            return false
        }
        val method_getNotchSize = class_HwNotchSizeUtil.getMethod("getNotchSize")
        val size = method_getNotchSize.invoke(class_HwNotchSizeUtil) as IntArray
        val notchWidth = size[0]
        val notchHeight = size[1]
        calculateSafeInset(activity, notchHeight, inset)
        return true
    }

    @Throws(ClassNotFoundException::class, IllegalAccessException::class, InstantiationException::class, InvocationTargetException::class, NoSuchMethodException::class)
    private fun getSafeInset_Oppo(activity: Activity, inset: SafeInset): Boolean {
        if (!activity.packageManager.hasSystemFeature("com.oppo.feature.screen.heteromorphism")) {
            return false
        }
        @SuppressLint("PrivateApi") val cls = Class.forName("android.os.SystemProperties")
        val hideMethod = cls.getMethod("get", String::class.java)
        val `object` = cls.newInstance()
        val value = hideMethod.invoke(`object`, "ro.oppo.screen.heteromorphism") as String

        // [378,0:702,80]
        val texts = value.split("[,:]").toTypedArray()
        var values: IntArray? = IntArray(texts.size)
        try {
            for (i in texts.indices) {
                values!![i] = texts[i].toInt()
            }
        } catch (e: NumberFormatException) {
            values = null
        }
        if (values != null && values.size == 4) {
            val notchHeight = values[3]
            calculateSafeInset(activity, notchHeight, inset)
        }
        return true
    }

    fun getSafeInset(activity: Activity): SafeInset {
        val inset = SafeInset()
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.P) {
            try {
                // https://stackoverflow.com/questions/51743622/how-to-handle-notchdisplay-cutout-in-android-api-lower-than-28
                if (getSafeInset_Oppo(activity, inset)) {
                    return inset
                }
                if (getSafeInset_Huawei(activity, inset)) {
                    return inset
                }
            } catch (ex: Exception) {
                _logger.error("getSafeInset", ex)
            }
        } else {
            val decorView = activity.window.decorView
            val insets = decorView.rootWindowInsets
            val cutout = insets.displayCutout
            if (cutout == null) {
                // Doesn't have cutout.
            } else {
                inset.left = cutout.safeInsetLeft
                inset.right = cutout.safeInsetRight
                inset.top = cutout.safeInsetTop
                inset.bottom = cutout.safeInsetBottom
            }
        }
        return inset
    }

    private fun sendMail(context: Context, recipient: String, subject: String, body: String): Boolean {
        val intent = Intent(Intent.ACTION_SEND) //
            .setType("message/rfc822") //
            .putExtra(Intent.EXTRA_EMAIL, arrayOf(recipient)) //
            .putExtra(Intent.EXTRA_SUBJECT, subject) //
            .putExtra(Intent.EXTRA_TEXT, body)
        try {
            // Fix Calling startActivity() from outside of an Activity  context requires the
            // FLAG_ACTIVITY_NEW_TASK flag. Is this really what you want?
            val chooserIntent = Intent.createChooser(intent, "Send mail...")
            chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
            context.startActivity(chooserIntent)
            return true
        } catch (ex: ActivityNotFoundException) {
            // There are no email clients installed.
            ex.printStackTrace()
        }
        return false
    }

    /// This methods only test if internet is connected or connecting.
    private fun isInternetAvailable(context: Context): Boolean {
        val connectivityManager = (context.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager)
        val info = connectivityManager.activeNetworkInfo
        return info != null && info.isConnectedOrConnecting
    }

    /// https://stackoverflow.com/questions/9570237/android-check-internet-connection
    /// https://stackoverflow.com/questions/2758612/executorservice-that-interrupts-tasks-after-a-timeout
    private fun testConnection(context: Context, hostName: String, timeOut: Float): Single<Boolean> {
        return Single
            .create<Boolean> { emitter ->
                try {
                    val isAvailable = isInternetAvailable(context)
                    if (!isAvailable) {
                        emitter.onSuccess(false)
                        return@create
                    }
                    val address = InetAddress.getByName(hostName)
                    emitter.onSuccess(address.toString() != "")
                } catch (ex: UnknownHostException) {
                    if (emitter.isDisposed) {
                        // Time-out.
                    } else {
                        emitter.onError(ex)
                    }
                }
            }
            .subscribeOn(Schedulers.io())
            .timeout((timeOut * 1000).toLong(), TimeUnit.MILLISECONDS)
    }

    private fun showInstantPrompt(url: String, referrer: String, activity: Activity) {
        val intent = Intent(Intent.ACTION_VIEW, Uri.parse(url)).addCategory(Intent.CATEGORY_BROWSABLE)
        com.google.android.gms.instantapps.InstantApps.showInstallPrompt(activity, intent, INSTALL_APP_REQUEST_CODE, referrer)
    }
}