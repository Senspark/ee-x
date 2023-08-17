package com.ee

import android.app.Activity
import android.app.Application
import android.util.Log
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.internal.deserialize
import com.google.android.gms.auth.api.signin.GoogleSignIn
import com.google.android.gms.auth.api.signin.GoogleSignInAccount
import com.google.android.gms.auth.api.signin.GoogleSignInClient
import com.google.android.gms.auth.api.signin.GoogleSignInOptions
import com.google.android.gms.games.AchievementsClient
import com.google.android.gms.games.Games
import com.google.android.gms.games.LeaderboardsClient
import com.google.android.gms.games.PlayGames
import com.google.android.gms.games.achievement.Achievement
import kotlinx.serialization.Serializable
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class PlayBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = PlayBridge::class.java.name
        private const val kPrefix = "PlayBridge"
        private const val kIsLoggedIn = "${kPrefix}IsLoggedIn"
        private const val kLogIn = "${kPrefix}LogIn"
        private const val kLogOut = "${kPrefix}LogOut"
        private const val kShowAchievements = "${kPrefix}ShowAchievements"
        private const val kIncrementAchievement = "${kPrefix}IncreaseAchievement"
        private const val kUnlockAchievement = "${kPrefix}UnlockAchievement"
        private const val kShowLeaderboard = "${kPrefix}ShowLeaderboard"
        private const val kShowAllLeaderboards = "${kPrefix}ShowAllLeaderboards"
        private const val kSubmitScore = "${kPrefix}SubmitScore"
        private const val kPushToCloud = "${kPrefix}PushToCloud"
        private const val kPullFromCloud = "${kPrefix}PullFromCloud"
        private const val kDeleteCloud = "${kPrefix}DeleteCloud"
    }

    private val _options = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN
    private var _client: GoogleSignInClient? = null
    private val _cloudSave: CloudSave

    init {
        Log.d(kTag, "nhanc18 constructor begin: application = $_application activity = $_activity")
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        _cloudSave = CloudSave()
        if (_activity != null) {
            _cloudSave.onCreate(_activity!!, _logger)
        }
        registerHandlers()
        Thread.runOnMainThread {
            _activity?.let { activity ->
                _client = GoogleSignIn.getClient(activity, _options)
            }
        }
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        Log.d(kTag, "nhanc18 constructor begin: application = $_application activity = $_activity")
        _activity = activity
        _client = GoogleSignIn.getClient(activity, _options)
        _cloudSave.onCreate(activity, _logger)
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

    @Serializable
    private class LogInRequest(
        val silently: Boolean
    )

    @Serializable
    private class IncrementAchievementRequest(
        val achievement_id: String,
        val increment: Double
    )

    @Serializable
    private class UnlockAchievementRequest(
        val achievement_id: String
    )

    @Serializable
    private class ShowLeaderboardRequest(
        val leaderboard_id: String
    )

    @Serializable
    private class SubmitScoreRequest(
        val leaderboard_id: String,
        val score: Long
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kIsLoggedIn) {
            Utils.toString(isLoggedIn)
        }
        _bridge.registerAsyncHandler(kLogIn) { message ->
            val request = deserialize<LogInRequest>(message)
            suspendCoroutine { cont ->
                logIn(request.silently) { successful ->
                    cont.resume(Utils.toString(successful))
                }
            }
        }
        _bridge.registerAsyncHandler(kLogOut) {
            suspendCoroutine { cont ->
                logOut { successful ->
                    cont.resume(Utils.toString(successful))
                }
            }
        }
        _bridge.registerHandler(kShowAchievements) {
            showAchievements()
            ""
        }
        _bridge.registerHandler(kIncrementAchievement) { message ->
            val request = deserialize<IncrementAchievementRequest>(message)
            incrementAchievement(request.achievement_id, request.increment)
            ""
        }
        _bridge.registerHandler(kUnlockAchievement) { message ->
            val request = deserialize<UnlockAchievementRequest>(message)
            unlockAchievement(request.achievement_id)
            ""
        }
        _bridge.registerHandler(kShowLeaderboard) { message ->
            val request = deserialize<ShowLeaderboardRequest>(message)
            showLeaderboard(request.leaderboard_id)
            ""
        }
        _bridge.registerHandler(kShowAllLeaderboards) {
            showAllLeaderboards()
            ""
        }
        _bridge.registerHandler(kSubmitScore) { message ->
            val request = deserialize<SubmitScoreRequest>(message)
            submitScore(request.leaderboard_id, request.score)
            ""
        }
        _bridge.registerAsyncHandler(kPushToCloud) { message ->
            suspendCoroutine { cont ->
                pushToCloud(message) { successful ->
                    cont.resume(Utils.toString(successful))
                }
            }
        }
        _bridge.registerAsyncHandler(kPullFromCloud) {
            suspendCoroutine { cont ->
                pullFromCloud { jsonData ->
                    cont.resume(jsonData)
                }
            }
        }
        _bridge.registerAsyncHandler(kDeleteCloud) {
            suspendCoroutine { cont ->
                deleteCloud { successful ->
                    cont.resume(Utils.toString(successful))
                }
            }
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kIsLoggedIn)
        _bridge.deregisterHandler(kLogIn)
        _bridge.deregisterHandler(kLogOut)
        _bridge.deregisterHandler(kShowAchievements)
        _bridge.deregisterHandler(kIncrementAchievement)
        _bridge.deregisterHandler(kUnlockAchievement)
        _bridge.deregisterHandler(kShowLeaderboard)
        _bridge.deregisterHandler(kShowAllLeaderboards)
        _bridge.deregisterHandler(kSubmitScore)
        _bridge.deregisterHandler(kPushToCloud)
        _bridge.deregisterHandler(kPullFromCloud)
        _bridge.deregisterHandler(kDeleteCloud)
    }

    private val signInAccount: GoogleSignInAccount?
        @UiThread get() = GoogleSignIn.getLastSignedInAccount(_application)

    private val achievementsClient: AchievementsClient?
        @UiThread get() {
            val activity = _activity ?: return null
            val account = signInAccount ?: return null
            return PlayGames.getAchievementsClient(activity)
        }

    private val leaderboardsClient: LeaderboardsClient?
        @UiThread get() {
            val activity = _activity ?: return null
            val account = signInAccount ?: return null
            return PlayGames.getLeaderboardsClient(activity)
        }

    // Check whether player is already signed in
    // https://developers.google.com/games/services/android/signin
    private val isLoggedIn: Boolean
        @UiThread get() = GoogleSignIn.hasPermissions(signInAccount, *_options.scopeArray)

    @AnyThread
    private fun logIn(silently: Boolean, callback: (successful: Boolean) -> Unit) {
        _logger.debug("$kTag: ${this::logIn.name}: silently = $silently")
        Thread.runOnMainThread {
            val activity = _activity ?: return@runOnMainThread
            val client = _client ?: throw IllegalArgumentException("Client is null")
            if (silently) {
                client.silentSignIn().addOnCompleteListener { task ->
                    callback(task.isSuccessful)
                }
            } else {
                val code = 1
                InvisibleActivity.Builder(activity)
                    .onStart { innerActivity ->
                        innerActivity.startActivityForResult(client.signInIntent, code)
                    }
                    .onFinish { requestCode, resultCode, _ ->
                        assert(requestCode == code)
                        val successful = resultCode == Activity.RESULT_OK
                        callback(successful)
                    }
                    .process()
            }
        }
    }

    @AnyThread
    private fun logOut(callback: (successful: Boolean) -> Unit) {
        _logger.debug("$kTag: ${this::logOut.name}")
        Thread.runOnMainThread {
            val client = _client ?: throw IllegalArgumentException("Client is null")
            client.signOut().addOnCompleteListener { task ->
                callback(task.isSuccessful)
            }
        }
    }

    @AnyThread
    private fun showAchievements() {
        _logger.debug("$kTag: ${this::showAchievements.name}")
        Thread.runOnMainThread {
            if (!isLoggedIn) {
                return@runOnMainThread
            }
            val client = achievementsClient ?: return@runOnMainThread
            client.achievementsIntent.addOnSuccessListener { intent ->
                val activity = _activity ?: return@addOnSuccessListener
                val code = 1
                InvisibleActivity.Builder(activity)
                    .onStart { innerActivity ->
                        innerActivity.startActivityForResult(intent, code)
                    }
                    .onFinish { requestCode, _, _ ->
                        assert(requestCode == code)
                        // OK.
                    }
                    .process()
            }
        }
    }

    // Load all achievements and check increment
    // Ref: https://stackoverflow.com/questions/23848014/google-play-game-services-unlock
    // -achievement-store-unlock-in-game-or-call-unlo/23853222#23853222
    @AnyThread
    private fun incrementAchievement(achievementId: String, percent: Double) {
        Thread.runOnMainThread {
            if (!isLoggedIn) {
                return@runOnMainThread
            }
            val client = achievementsClient ?: return@runOnMainThread
            client.load(true).addOnCompleteListener { task ->
                val buffer = task.result?.get() ?: return@addOnCompleteListener
                val bufferSize = buffer.count
                for (i in 0 until bufferSize) {
                    val achievement = buffer[i]
                    if (achievement.type != Achievement.TYPE_INCREMENTAL) {
                        continue
                    }
                    if (achievement.achievementId != achievementId) {
                        continue
                    }
                    val currStep = achievement.currentSteps
                    val totalStep = achievement.totalSteps
                    val reportStep = (percent * totalStep).toInt()
                    val increment = reportStep - currStep
                    if (increment > 0) {
                        client.increment(achievementId, increment)
                    }
                    break
                }
                buffer.release()
            }
        }
    }

    @AnyThread
    private fun unlockAchievement(achievementId: String) {
        Thread.runOnMainThread {
            if (!isLoggedIn) {
                return@runOnMainThread
            }
            val client = achievementsClient ?: return@runOnMainThread
            client.unlock(achievementId)
        }
    }

    @AnyThread
    private fun showLeaderboard(leaderboardId: String) {
        _logger.debug("$kTag: ${this::showLeaderboard.name}: id = $leaderboardId")
        Thread.runOnMainThread {
            if (!isLoggedIn) {
                return@runOnMainThread
            }
            val client = leaderboardsClient ?: return@runOnMainThread
            client.getLeaderboardIntent(leaderboardId).addOnSuccessListener { intent ->
                val activity = _activity ?: return@addOnSuccessListener
                val code = 1
                InvisibleActivity.Builder(activity)
                    .onStart { innerActivity ->
                        innerActivity.startActivityForResult(intent, code)
                    }
                    .onFinish { requestCode, _, _ ->
                        assert(requestCode == code)
                        // OK.
                    }
                    .process()
            }
        }
    }

    @AnyThread
    private fun showAllLeaderboards() {
        _logger.debug("$kTag: ${this::showAllLeaderboards.name}")
        Thread.runOnMainThread {
            if (!isLoggedIn) {
                return@runOnMainThread
            }
            val client = leaderboardsClient ?: return@runOnMainThread
            client.allLeaderboardsIntent.addOnSuccessListener { intent ->
                val activity = _activity ?: return@addOnSuccessListener
                val code = 1
                InvisibleActivity.Builder(activity)
                    .onStart { innerActivity ->
                        innerActivity.startActivityForResult(intent, code)
                    }
                    .onFinish { requestCode, _, _ ->
                        assert(requestCode == code)
                        // OK.
                    }
                    .process()
            }
        }
    }

    @AnyThread
    private fun submitScore(leaderboardId: String, score: Long) {
        Thread.runOnMainThread {
            if (!isLoggedIn) {
                return@runOnMainThread
            }
            val client = leaderboardsClient ?: return@runOnMainThread
            client.submitScore(leaderboardId, score)
        }
    }

    @AnyThread
    private fun pushToCloud(jsonData: String, callback: (Boolean) -> Unit) {
        Log.d(kTag, "${this::pushToCloud.name}: nhanc18 $jsonData")
        Thread.runOnMainThread {
            _cloudSave.push(jsonData) { successful ->
                Log.d(kTag, "${this::pushToCloud.name}: nhanc18 $successful")
                callback(successful)
            }
        }
    }

    @AnyThread
    private fun pullFromCloud(callback: (jsonData: String) -> Unit) {
        Log.d(kTag, "${this::pullFromCloud.name}: nhanc18")
        Thread.runOnMainThread {
            _cloudSave.pull { jsonData ->
                Log.d(kTag, "${this::pullFromCloud.name}: nhanc18 $jsonData")
                callback(jsonData)
            }
        }
    }

    @AnyThread
    private fun deleteCloud(callback: (Boolean) -> Unit) {
        Log.d(kTag, "${this::deleteCloud.name}: nhanc18")
    }
}