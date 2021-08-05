package com.ee

import android.app.Activity
import android.app.Application
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.facebook.AccessToken
import com.facebook.AccessTokenTracker
import com.facebook.CallbackManager
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.FacebookSdk
import com.facebook.GraphRequest
import com.facebook.HttpMethod
import com.facebook.Profile
import com.facebook.ProfileTracker
import com.facebook.login.LoginManager
import com.facebook.login.LoginResult
import com.facebook.share.Sharer
import com.facebook.share.model.ShareContent
import com.facebook.share.model.ShareLinkContent
import com.facebook.share.model.SharePhoto
import com.facebook.share.model.SharePhotoContent
import com.facebook.share.model.ShareVideo
import com.facebook.share.model.ShareVideoContent
import com.facebook.share.widget.ShareDialog
import kotlinx.serialization.Serializable
import java.io.File
import java.io.UnsupportedEncodingException
import java.net.URLEncoder
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
class FacebookBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FacebookBridge::class.java.name
        private const val kPrefix = "FacebookBridge"
        private const val kRegisterNotifications = "${kPrefix}RegisterNotifications"
        private const val kIsLoggedIn = "${kPrefix}IsLoggedIn"
        private const val kLogIn = "${kPrefix}LogIn"
        private const val kLogOut = "${kPrefix}LogOut"
        private const val kGetAccessToken = "${kPrefix}GetAccessToken"
        private const val kOnProfileChanged = "${kPrefix}OnProfileChanged"
        private const val kGraphRequest = "${kPrefix}GraphRequest"
        private const val kSendRequest = "${kPrefix}SendRequest"
        private const val kShareContent = "${kPrefix}ShareContent"
    }

    private val _callbackManager = CallbackManager.Factory.create()
    private val _loginManager = LoginManager.getInstance()
    private val _accessTokenTracker: AccessTokenTracker
    private val _profileTracker: ProfileTracker

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        _accessTokenTracker = object : AccessTokenTracker() {
            override fun onCurrentAccessTokenChanged(oldAccessToken: AccessToken?,
                                                     currentAccessToken: AccessToken?) {
                _logger.debug("$kTag: ${this::onCurrentAccessTokenChanged.name}")
            }
        }
        _profileTracker = object : ProfileTracker() {
            override fun onCurrentProfileChanged(oldProfile: Profile?, currentProfile: Profile?) {
                _logger.debug("$kTag: ${this::onCurrentProfileChanged.name}")
                val dict: MutableMap<String, Any> = HashMap()
                if (currentProfile != null) {
                    dict["userId"] = currentProfile.id ?: ""
                    dict["firstName"] = currentProfile.firstName ?: ""
                    dict["middleName"] = currentProfile.middleName ?: ""
                    dict["lastName"] = currentProfile.lastName ?: ""
                    dict["name"] = currentProfile.name ?: ""
                    dict["picture"] = currentProfile.getProfilePictureUri(128, 128).toString()
                }
                // FIXME: may cause java.lang.UnsatisfiedLinkError if C++ library has not loaded yet.
                // _bridge.callCpp(k__onProfileChanged, JsonUtils.convertDictionaryToString(dict));
            }
        }

        // com.facebook.UserSettingsManager: Please set a value for AutoLogAppEventsEnabled.
        // Set the flag to TRUE if you want to collect app install, app launch and in-app purchase events automatically.
        // To request user consent before collecting data, set the flag value to FALSE, then change to TRUE once user consent is received.
        // Learn more: https://developers.facebook.com/docs/app-events/getting-started-app-events-android#disable-auto-events.
        FacebookSdk.setAutoLogAppEventsEnabled(true)

        // com.facebook.UserSettingsManager: You haven't set a value for AdvertiserIDCollectionEnabled.
        // Set the flag to TRUE if you want to collect Advertiser ID for better advertising and analytics results.
        // To request user consent before collecting data, set the flag value to FALSE, then change to TRUE once user consent is received.
        // Learn more: https://developers.facebook.com/docs/app-events/getting-started-app-events-android#disable-auto-events.
        FacebookSdk.setAdvertiserIDCollectionEnabled(true)
        registerHandlers()
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
    }

    private fun registerNotifications() {
        _profileTracker.startTracking()
        _accessTokenTracker.startTracking()
    }

    @Serializable
    private class LogInRequest(
        val permissions: List<String>,
    )

    @Serializable
    @Suppress("unused")
    private class LogInResponse(
        val successful: Boolean,
        val canceled: Boolean,
        val errorMessage: String,
    )

    @Serializable
    @Suppress("unused")
    private class GetAccessTokenResponse(
        val token: String,
        val applicationId: String,
        val userId: String
    )

    @Serializable
    private class GraphRequestRequest(
        val path: String,
        val parameters: Map<String, String>,
    )

    @Serializable
    @Suppress("unused")
    private class GraphRequestResponse(
        val successful: Boolean,
        val response: String,
        val errorMessage: String,
    )

    @Serializable
    private class ShareContentRequest(
        val type: Int,
        val url: String,
    )

    @Serializable
    @Suppress("unused")
    private class ShareContentResponse(
        val successful: Boolean,
        val canceled: Boolean,
        val errorMessage: String,
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kRegisterNotifications) {
            registerNotifications()
            ""
        }
        _bridge.registerHandler(kIsLoggedIn) {
            Utils.toString(isLoggedIn)
        }
        _bridge.registerAsyncHandler(kLogIn) { message ->
            val request = deserialize<LogInRequest>(message)
            val response = suspendCoroutine<LogInResponse> { cont ->
                logIn(request.permissions, object : FacebookCallback<LoginResult> {
                    override fun onSuccess(result: LoginResult) {
                        cont.resume(LogInResponse(successful = true, canceled = false, ""))
                    }

                    override fun onCancel() {
                        cont.resume(LogInResponse(successful = false, canceled = true, ""))
                    }

                    override fun onError(error: FacebookException) {
                        cont.resume(
                            LogInResponse(
                                successful = false, canceled = false,
                                error.localizedMessage ?: ""
                            )
                        )
                    }
                })
            }
            response.serialize()
        }
        _bridge.registerAsyncHandler(kLogOut) {
            logOut()
            ""
        }
        _bridge.registerHandler(kGetAccessToken) {
            val token = accessToken ?: return@registerHandler ""
            val response = GetAccessTokenResponse(
                token.token,
                token.applicationId,
                token.userId
            )
            response.serialize()
        }
        _bridge.registerAsyncHandler(kGraphRequest) { message ->
            val request = deserialize<GraphRequestRequest>(message)
            val parameters = Bundle()
            for ((key, value) in request.parameters) {
                parameters.putString(key, value)
            }
            val response = suspendCoroutine<GraphRequestResponse> { cont ->
                graphRequest(request.path, parameters) { result ->
                    if (result.error != null) {
                        cont.resume(
                            GraphRequestResponse(
                                false,
                                "",
                                result.error?.errorMessage ?: ""
                            )
                        )
                    } else {
                        cont.resume(GraphRequestResponse(true, result.rawResponse ?: "", ""))
                    }
                }
            }
            response.serialize()
        }
        /*
        _bridge.registerHandler(kSendRequest) { message ->
            sendRequest(message)
        }
         */
        _bridge.registerAsyncHandler(kShareContent) { message ->
            val request = deserialize<ShareContentRequest>(message)
            val response = suspendCoroutine<ShareContentResponse> { cont ->
                val callback = object : FacebookCallback<Sharer.Result> {
                    override fun onSuccess(result: Sharer.Result) {
                        cont.resume(ShareContentResponse(successful = true, canceled = false, ""))
                    }

                    override fun onCancel() {
                        cont.resume(ShareContentResponse(successful = false, canceled = true, ""))
                    }

                    override fun onError(error: FacebookException) {
                        cont.resume(
                            ShareContentResponse(
                                successful = false, canceled = false,
                                error.localizedMessage ?: ""
                            )
                        )
                    }
                }
                when (request.type) {
                    0 -> shareLinkContent(request.url, callback)
                    1 -> sharePhotoContent(request.url, callback)
                    2 -> shareVideoContent(request.url, callback)
                    else -> throw IllegalStateException("Invalid share type")
                }
            }
            response.serialize()
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kRegisterNotifications)
        _bridge.deregisterHandler(kIsLoggedIn)
        _bridge.deregisterHandler(kLogIn)
        _bridge.deregisterHandler(kLogOut)
        _bridge.deregisterHandler(kGetAccessToken)
        _bridge.deregisterHandler(kGraphRequest)
        // _bridge.deregisterHandler(kSendRequest)
        _bridge.deregisterHandler(kShareContent)
    }

    private val isLoggedIn: Boolean
        get() {
            val token = AccessToken.getCurrentAccessToken()
            return token != null && !token.isExpired
        }

    private fun logIn(permissions: List<String>, callback: FacebookCallback<LoginResult>) {
        Thread.runOnMainThread {
            val activity = _activity ?: throw IllegalStateException("Activity is null")
            InvisibleActivity.Builder(activity)
                .onStart { innerActivity ->
                    _loginManager.registerCallback(_callbackManager, callback)
                    _loginManager.logInWithReadPermissions(innerActivity, permissions)
                }
                .onFinish { requestCode, resultCode, data ->
                    _callbackManager.onActivityResult(requestCode, resultCode, data)
                }
                .process()
        }
    }

    private suspend fun logOut() {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _loginManager.logOut()
                cont.resume(Unit)
            }
        }
    }

    private
    val accessToken: AccessToken?
        get() = AccessToken.getCurrentAccessToken()

    private fun graphRequest(path: String, parameters: Bundle, callback: GraphRequest.Callback) {
        Thread.runOnMainThread {
            val request = GraphRequest(accessToken, path, parameters, HttpMethod.GET, callback)
            request.executeAsync()
        }
    }

    /*
    private fun sendRequest(message: String): String {
        @Serializable
        class Request(
            val actionType: Int,
            val filter: Int,
            val title: String,
            val recipients: List<String>,
            val objectId: String,
            val data: String,
            val message: String,
            val tag: Int
        )

        val request = deserialize<Request>(message)
        val actionTypes = SparseArray<GameRequestContent.ActionType>()
        actionTypes.put(0, null)
        actionTypes.put(1, GameRequestContent.ActionType.SEND)
        actionTypes.put(2, GameRequestContent.ActionType.ASKFOR)
        actionTypes.put(3, GameRequestContent.ActionType.TURN)
        val filters = SparseArray<GameRequestContent.Filters>()
        filters.put(0, null)
        filters.put(1, GameRequestContent.Filters.APP_USERS)
        filters.put(2, GameRequestContent.Filters.APP_NON_USERS)
        sendRequest(
            actionTypes[request.actionType],
            filters[request.filter],
            request.title,
            request.recipients,
            request.objectId,
            request.data,
            request.message,
            FacebookRequestDelegate(_bridge, request.tag))
        return ""
    }
     */

    /*
    private fun sendRequest(actionType: GameRequestContent.ActionType,
                            filter: GameRequestContent.Filters,
                            title: String,
                            recipients: List<String>,
                            objectId: String,
                            data: String,
                            message: String,
                            delegate: FacebookCallback<GameRequestDialog.Result>) {
        val content = GameRequestContent.Builder()
            .setActionType(actionType)
            .setFilters(filter)
            .setTitle(title)
            .setObjectId(objectId)
            .setRecipients(recipients)
            .setData(data)
            .setMessage(message)
            .build()

        val activity = _activity ?: throw IllegalStateException("Activity is null")
        InvisibleActivity.Builder(activity)
            .onStart { innerActivity ->
                val dialog = GameRequestDialog(innerActivity)
                dialog.registerCallback(_callbackManager, delegate)
                dialog.show(content)
            }
            .onFinish { requestCode, resultCode, data ->
                _callbackManager.onActivityResult(requestCode, resultCode, data)
            }
            .process()
    }
     */

    private fun shareLinkContent(url: String, callback: FacebookCallback<Sharer.Result>) {
        Thread.runOnMainThread {
            val encodedUrl = try {
                URLEncoder.encode(url, "utf-8");
            } catch (ex: UnsupportedEncodingException) {
                url
            }
            val content = ShareLinkContent.Builder()
                .setContentUrl(Uri.parse(encodedUrl))
                .build()
            shareContent(content, callback)
        }
    }

    private fun sharePhotoContent(url: String, callback: FacebookCallback<Sharer.Result>) {
        Thread.runOnMainThread {
            val image = BitmapFactory.decodeFile(url)
            val photo = SharePhoto.Builder().setBitmap(image).build()
            val content = SharePhotoContent.Builder().addPhoto(photo).build()
            shareContent(content, callback)
        }
    }

    private fun shareVideoContent(url: String, callback: FacebookCallback<Sharer.Result>) {
        Thread.runOnMainThread {
            val video = File(url)
            val videoFileUri = Uri.fromFile(video)
            val videoContent = ShareVideo.Builder().setLocalUrl(videoFileUri).build()
            val content = ShareVideoContent.Builder().setVideo(videoContent).build()
            shareContent(content, callback)
        }
    }

    private fun shareContent(content: ShareContent<*, *>, callback: FacebookCallback<Sharer.Result>) {
        Thread.runOnMainThread {
            val activity = _activity ?: throw IllegalStateException("Activity is null")
            InvisibleActivity.Builder(activity)
                .onStart { innerActivity ->
                    val dialog = ShareDialog(innerActivity)
                    dialog.registerCallback(_callbackManager, callback)
                    dialog.show(content, ShareDialog.Mode.AUTOMATIC)
                }
                .onFinish { requestCode, resultCode, data ->
                    _callbackManager.onActivityResult(requestCode, resultCode, data)
                }
                .process()
        }
    }
}