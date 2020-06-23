package com.ee.play

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.InvisibleActivity
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.registerHandler
import com.google.android.gms.auth.api.signin.GoogleSignIn
import com.google.android.gms.auth.api.signin.GoogleSignInAccount
import com.google.android.gms.auth.api.signin.GoogleSignInClient
import com.google.android.gms.auth.api.signin.GoogleSignInOptions
import com.google.android.gms.games.AchievementsClient
import com.google.android.gms.games.Games
import com.google.android.gms.games.LeaderboardsClient
import com.google.android.gms.games.achievement.Achievement
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

@ImplicitReflectionSerializer
@UnstableDefault
class Play(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(Play::class.java.name)

        private const val kIsLoggedIn = "Play_isSignedIn"
        private const val kLogIn = "Play_signin"
        private const val kLogOut = "Play_signout"
        private const val kShowAchievements = "Play_showAchievements"
        private const val kIncreaseAchievement = "Play_increaseAchievement"
        private const val kUnlockAchievement = "Play_unlockAchievement"
        private const val kShowLeaderboard = "Play_showLeaderboard"
        private const val kShowAllLeaderboards = "Play_showAllLeaderboards"
        private const val kSubmitScore = "Play_submitScore"
        private const val kOnLoggedIn = "Play_onSignedIn"
    }

    private val _options = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN
    private var _client: GoogleSignInClient? = null

    init {
        registerHandlers()
        Thread.runOnMainThread(Runnable {
            _activity?.let { activity ->
                _client = GoogleSignIn.getClient(activity, _options)
            }
        })
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        _client = GoogleSignIn.getClient(activity, _options)
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

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kIsLoggedIn) {
            Utils.toString(isLoggedIn)
        }
        _bridge.registerHandler(kLogIn) { message ->
            @Serializable
            class Request(
                val silent_sign_in: Boolean
            )

            val request = deserialize<Request>(message)
            logIn(request.silent_sign_in) { successful ->
                _bridge.callCpp(kOnLoggedIn, Utils.toString(successful))
            }
            ""
        }
        _bridge.registerHandler(kLogOut) {
            logOut {
                // OK.
            }
            ""
        }
        _bridge.registerHandler(kShowAchievements) {
            showAchievements()
            ""
        }
        _bridge.registerHandler(kIncreaseAchievement) { message ->
            @Serializable
            class Request(
                val achievement_id: String,
                val increment: Double
            )

            val request = deserialize<Request>(message)
            incrementAchievement(request.achievement_id, request.increment)
            ""
        }
        _bridge.registerHandler(kUnlockAchievement) { message ->
            @Serializable
            class Request(
                val achievement_id: String
            )

            val request = deserialize<Request>(message)
            unlockAchievement(request.achievement_id)
            ""
        }
        _bridge.registerHandler(kShowLeaderboard) { message ->
            @Serializable
            class Request(
                val leaderboard_id: String
            )

            val request = deserialize<Request>(message)
            showLeaderboard(request.leaderboard_id)
            ""
        }
        _bridge.registerHandler(kShowAllLeaderboards) {
            showAllLeaderboards()
            ""
        }
        _bridge.registerHandler(kSubmitScore) { message ->
            @Serializable
            class Request(
                val leaderboard_id: String,
                val score: Long
            )

            val request = deserialize<Request>(message)
            submitScore(request.leaderboard_id, request.score)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kIsLoggedIn)
        _bridge.deregisterHandler(kLogIn)
        _bridge.deregisterHandler(kLogOut)
        _bridge.deregisterHandler(kShowAchievements)
        _bridge.deregisterHandler(kIncreaseAchievement)
        _bridge.deregisterHandler(kUnlockAchievement)
        _bridge.deregisterHandler(kShowLeaderboard)
        _bridge.deregisterHandler(kShowAllLeaderboards)
        _bridge.deregisterHandler(kSubmitScore)
    }

    private val signInAccount: GoogleSignInAccount?
        @UiThread get() = GoogleSignIn.getLastSignedInAccount(_context)

    private val achievementsClient: AchievementsClient?
        @UiThread get() {
            val activity = _activity ?: return null
            val account = signInAccount ?: return null
            return Games.getAchievementsClient(activity, account)
        }

    private val leaderboardsClient: LeaderboardsClient?
        @UiThread get() {
            val activity = _activity ?: return null
            val account = signInAccount ?: return null
            return Games.getLeaderboardsClient(activity, account)
        }

    // Check whether player is already signed in
    // https://developers.google.com/games/services/android/signin
    private val isLoggedIn: Boolean
        @UiThread get() = GoogleSignIn.hasPermissions(signInAccount, *_options.scopeArray)

    @UiThread
    private fun logIn(silently: Boolean, callback: (successful: Boolean) -> Unit) {
        _logger.debug("${this::logIn.name}: silently = $silently")
        val activity = _activity ?: return
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
                    assertThat(requestCode == code)
                    val successful = resultCode == Activity.RESULT_OK
                    callback(successful)
                }
                .process()
        }
    }

    @UiThread
    private fun logOut(callback: (successful: Boolean) -> Unit) {
        _logger.debug(this::logOut.name)
        val client = _client ?: throw IllegalArgumentException("Client is null")
        client.signOut().addOnCompleteListener { task ->
            callback(task.isSuccessful)
        }
    }

    @UiThread
    private fun showAchievements() {
        if (!isLoggedIn) {
            return
        }
        val client = achievementsClient ?: return
        client.achievementsIntent.addOnSuccessListener { intent ->
            val activity = _activity ?: return@addOnSuccessListener
            val code = 1
            InvisibleActivity.Builder(activity)
                .onStart { innerActivity ->
                    innerActivity.startActivityForResult(intent, code)
                }
                .onFinish { requestCode, _, _ ->
                    assertThat(requestCode == code)
                    // OK.
                }
                .process()
        }
    }

    // Load all achievements and check increment
    // Ref: https://stackoverflow.com/questions/23848014/google-play-game-services-unlock
    // -achievement-store-unlock-in-game-or-call-unlo/23853222#23853222
    @UiThread
    private fun incrementAchievement(achievementId: String, percent: Double) {
        if (!isLoggedIn) {
            return
        }
        val client = achievementsClient ?: return
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

    @UiThread
    private fun unlockAchievement(achievementId: String) {
        if (!isLoggedIn) {
            return
        }
        val client = achievementsClient ?: return
        client.unlock(achievementId)
    }

    @UiThread
    private fun showLeaderboard(leaderboardId: String) {
        if (!isLoggedIn) {
            return
        }
        val client = leaderboardsClient ?: return
        client.getLeaderboardIntent(leaderboardId).addOnSuccessListener { intent ->
            val activity = _activity ?: return@addOnSuccessListener
            val code = 1
            InvisibleActivity.Builder(activity)
                .onStart { innerActivity ->
                    innerActivity.startActivityForResult(intent, code)
                }
                .onFinish { requestCode, _, _ ->
                    assertThat(requestCode).isEqualTo(code)
                    // OK.
                }
                .process()
        }
    }

    @UiThread
    private fun showAllLeaderboards() {
        if (!isLoggedIn) {
            return
        }
        val client = leaderboardsClient ?: return
        client.allLeaderboardsIntent.addOnSuccessListener { intent ->
            val activity = _activity ?: return@addOnSuccessListener
            val code = 1
            InvisibleActivity.Builder(activity)
                .onStart { innerActivity ->
                    innerActivity.startActivityForResult(intent, code)
                }
                .onFinish { requestCode, _, _ ->
                    assertThat(requestCode).isEqualTo(code)
                    // OK.
                }
                .process()
        }
    }

    @UiThread
    private fun submitScore(leaderboardId: String, score: Long) {
        if (!isLoggedIn) {
            return
        }
        val client = leaderboardsClient ?: return
        client.submitScore(leaderboardId, score)
    }
}