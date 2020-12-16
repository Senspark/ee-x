package com.ee

import android.app.Activity
import android.app.Application
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.util.SparseArray
import androidx.annotation.AnyThread
import com.ee.internal.FacebookLoginDelegate
import com.ee.internal.FacebookRequestDelegate
import com.ee.internal.FacebookShareDelegate
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.facebook.AccessToken
import com.facebook.AccessTokenTracker
import com.facebook.CallbackManager
import com.facebook.FacebookCallback
import com.facebook.FacebookSdk
import com.facebook.GraphRequest
import com.facebook.HttpMethod
import com.facebook.Profile
import com.facebook.ProfileTracker
import com.facebook.login.LoginManager
import com.facebook.login.LoginResult
import com.facebook.share.Sharer
import com.facebook.share.model.GameRequestContent
import com.facebook.share.model.ShareContent
import com.facebook.share.model.ShareLinkContent
import com.facebook.share.model.SharePhoto
import com.facebook.share.model.SharePhotoContent
import com.facebook.share.model.ShareVideo
import com.facebook.share.model.ShareVideoContent
import com.facebook.share.widget.GameRequestDialog
import com.facebook.share.widget.ShareDialog
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.io.File

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@InternalSerializationApi
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
        private const val kShareLinkContent = "${kPrefix}ShareLinkContent"
        private const val kSharePhotoContent = "${kPrefix}SharePhotoContent"
        private const val kShareVideoContent = "${kPrefix}ShareVideoContent"

        @AnyThread
        @InternalSerializationApi
        fun convertAccessTokenToString(token: AccessToken?): String {
            if (token == null) {
                return ""
            }
            @Serializable
            @Suppress("unused")
            class Response(
                val token: String,
                val applicationId: String,
                val userId: String
            )

            val response = Response(token.token, token.applicationId, token.userId)
            return response.serialize()
        }
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
                    dict["userId"] = currentProfile.id
                    dict["firstName"] = currentProfile.firstName
                    dict["middleName"] = if (currentProfile.middleName == null) "" else currentProfile.middleName
                    dict["lastName"] = currentProfile.lastName
                    dict["name"] = currentProfile.name
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
        _logger.info("$kTag: constructor end.")
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

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kRegisterNotifications) {
            registerNotifications()
            ""
        }
        _bridge.registerHandler(kIsLoggedIn) {
            Utils.toString(isLoggedIn)
        }
        _bridge.registerHandler(kLogIn) { message ->
            @Serializable
            class Request(
                val permissions: List<String>,
                val tag: Int
            )

            val request = deserialize<Request>(message)
            logIn(request.permissions, FacebookLoginDelegate(_bridge, request.tag))
            ""
        }
        _bridge.registerHandler(kLogOut) {
            logOut()
            ""
        }
        _bridge.registerHandler(kGetAccessToken) {
            val token = accessToken
            convertAccessTokenToString(token)
        }
        _bridge.registerHandler(kGraphRequest) { message ->
            graphRequest(message)
        }
        _bridge.registerHandler(kSendRequest) { message ->
            sendRequest(message)
        }
        _bridge.registerHandler(kShareLinkContent) { message ->
            @Serializable
            class Request(
                val url: String,
                val tag: Int
            )

            val request = deserialize<Request>(message)
            shareLinkContent(request.url, FacebookShareDelegate(_bridge, request.tag))
            ""
        }
        _bridge.registerHandler(kSharePhotoContent) { message ->
            @Serializable
            class Request(
                val url: String,
                val tag: Int
            )

            val request = deserialize<Request>(message)
            sharePhotoContent(request.url, FacebookShareDelegate(_bridge, request.tag))
            ""
        }
        _bridge.registerHandler(kShareVideoContent) { message ->
            @Serializable
            class Request(
                val url: String,
                val tag: Int
            )

            val request = deserialize<Request>(message)
            shareVideoContent(request.url, FacebookShareDelegate(_bridge, request.tag))
            ""
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
        _bridge.deregisterHandler(kSendRequest)
        _bridge.deregisterHandler(kShareLinkContent)
        _bridge.deregisterHandler(kSharePhotoContent)
        _bridge.deregisterHandler(kShareVideoContent)
    }

    private val isLoggedIn: Boolean
        get() = AccessToken.getCurrentAccessToken() != null

    private fun logIn(permissions: List<String>, callback: FacebookCallback<LoginResult>) {
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

    private fun logOut() {
        _loginManager.logOut()
    }

    private
    val accessToken: AccessToken?
        get() = AccessToken.getCurrentAccessToken()

    private fun k__onGraphSuccess(tag: Int): String {
        return "FacebookGraphDelegate_onSuccess_$tag"
    }

    private fun k__onGraphFailure(tag: Int): String {
        return "FacebookGraphDelegate_onFailure_$tag"
    }

    private fun graphRequest(message: String): String {
        @Serializable
        class Request(
            val path: String,
            val parameters: Map<String, String>,
            val tag: Int
        )

        val request = deserialize<Request>(message)
        val parameters = Bundle()
        for ((key, value) in request.parameters) {
            parameters.putString(key, value)
        }
        val callback = GraphRequest.Callback { response ->
            if (response.error != null) {
                _bridge.callCpp(k__onGraphFailure(request.tag), response.error.errorMessage)
            } else {
                _bridge.callCpp(k__onGraphSuccess(request.tag), response.rawResponse)
            }
        }
        graphRequest(request.path, parameters, callback)
        return ""
    }

    private fun graphRequest(path: String, parameters: Bundle, callback: GraphRequest.Callback) {
        val request = GraphRequest(accessToken, path, parameters, HttpMethod.GET, callback)
        request.executeAsync()
    }

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

    private fun shareLinkContent(url: String, delegate: FacebookCallback<Sharer.Result>) {
//        String encodedUrl;
//        try {
//            encodedUrl = URLEncoder.encode(url, "utf-8");
//        } catch (UnsupportedEncodingException e) {
//            encodedUrl = url;
//        }
        val content = ShareLinkContent.Builder()
            .setContentUrl(Uri.parse(url))
            .build()
        shareContent(content, delegate)
    }

    private fun sharePhotoContent(url: String, delegate: FacebookCallback<Sharer.Result>) {
        val image = BitmapFactory.decodeFile(url)
        val photo = SharePhoto.Builder().setBitmap(image).build()
        val content = SharePhotoContent.Builder().addPhoto(photo).build()
        shareContent(content, delegate)
    }

    private fun shareVideoContent(url: String, delegate: FacebookCallback<Sharer.Result>) {
        val video = File(url)
        val videoFileUri = Uri.fromFile(video)
        val videoContent = ShareVideo.Builder().setLocalUrl(videoFileUri).build()
        val content = ShareVideoContent.Builder().setVideo(videoContent).build()
        shareContent(content, delegate)
    }

    private fun shareContent(content: ShareContent<*, *>, delegate: FacebookCallback<Sharer.Result>) {
        val activity = _activity ?: throw IllegalStateException("Activity is null")
        InvisibleActivity.Builder(activity)
            .onStart { innerActivity ->
                val dialog = ShareDialog(innerActivity)
                dialog.registerCallback(_callbackManager, delegate)
                dialog.show(content, ShareDialog.Mode.AUTOMATIC)
            }
            .onFinish { requestCode, resultCode, data ->
                _callbackManager.onActivityResult(requestCode, resultCode, data)
            }
            .process()
    }
}