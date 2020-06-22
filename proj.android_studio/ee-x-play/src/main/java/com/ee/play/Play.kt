package com.ee.play

import android.app.Activity
import android.content.Context
import android.content.Intent
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.registerHandler
import com.google.android.gms.auth.api.signin.GoogleSignIn
import com.google.android.gms.auth.api.signin.GoogleSignInClient
import com.google.android.gms.auth.api.signin.GoogleSignInOptions
import com.google.android.gms.games.AchievementsClient
import com.google.android.gms.games.Games
import com.google.android.gms.games.LeaderboardsClient
import com.google.android.gms.games.achievement.Achievement
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

        private const val k_isSignedIn = "Play_isSignedIn"
        private const val k_signin = "Play_signin"
        private const val k_signout = "Play_signout"
        private const val k_showAchievements = "Play_showAchievements"
        private const val k_increaseAchievement = "Play_increaseAchievement"
        private const val k_unlockAchievement = "Play_unlockAchievement"
        private const val k_showLeaderboard = "Play_showLeaderboard"
        private const val k_showAllLeaderboards = "Play_showAllLeaderboards"
        private const val k_submitScore = "Play_submitScore"
        private const val k_onSignedIn = "Play_onSignedIn"
        private const val RC_SIGN_IN = 201912001
        private const val RC_ACHIEVEMENT_UI = 201912002
        private const val RC_LEADER_BOARD_UI = 201912003
    }

    private var _signInClient: GoogleSignInClient? = null
    private var _signInOptions: GoogleSignInOptions? = null

    init {
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        val options = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN
        _signInOptions = options
        _signInClient = GoogleSignIn.getClient(activity, options)
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
        _bridge.registerHandler(k_isSignedIn) {
            Utils.toString(isSignedIn())
        }
        _bridge.registerHandler(k_signin) { message ->
            @Serializable
            class Request(
                val silent_sign_in: Boolean
            )

            val request = deserialize<Request>(message)
            signIn(request.silent_sign_in)
            ""
        }
        _bridge.registerHandler(k_signout) {
            signOut()
            ""
        }
        _bridge.registerHandler(k_showAchievements) {
            showAchievements()
            ""
        }
        _bridge.registerHandler(k_increaseAchievement) { message ->
            @Serializable
            class Request(
                val achievement_id: String,
                val increment: Double
            )

            val request = deserialize<Request>(message)
            incrementAchievement(request.achievement_id, request.increment)
            ""
        }
        _bridge.registerHandler(k_unlockAchievement) { message ->
            @Serializable
            class Request(
                val achievement_id: String
            )

            val request = deserialize<Request>(message)
            unlockAchievement(request.achievement_id)
            ""
        }
        _bridge.registerHandler(k_showLeaderboard) { message ->
            @Serializable
            class Request(
                val leaderboard_id: String
            )

            val request = deserialize<Request>(message)
            showLeaderboard(request.leaderboard_id)
            ""
        }
        _bridge.registerHandler(k_showAllLeaderboards) {
            showAllLeaderboards()
            ""
        }
        _bridge.registerHandler(k_submitScore) { message ->
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
        _bridge.deregisterHandler(k_isSignedIn)
        _bridge.deregisterHandler(k_signin)
        _bridge.deregisterHandler(k_signout)
        _bridge.deregisterHandler(k_showAchievements)
        _bridge.deregisterHandler(k_increaseAchievement)
        _bridge.deregisterHandler(k_unlockAchievement)
        _bridge.deregisterHandler(k_showLeaderboard)
        _bridge.deregisterHandler(k_showAllLeaderboards)
        _bridge.deregisterHandler(k_submitScore)
    }

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        if (requestCode == RC_SIGN_IN) {
            val success = responseCode == Activity.RESULT_OK
            _bridge.callCpp(k_onSignedIn, Utils.toString(success))
            return true
        }
        return false
    }

    // Check whether player is already signed in
    // https://developers.google.com/games/services/android/signin
    private fun isSignedIn(): Boolean {
        val options = _signInOptions ?: return false
        val account = GoogleSignIn.getLastSignedInAccount(_activity)
        return account != null && GoogleSignIn.hasPermissions(account, *options.scopeArray)
    }

    private fun signIn(silentSignIn: Boolean) {
        _logger.debug("signIn $silentSignIn")
        if (silentSignIn) {
            signInSilently()
        } else {
            startSignInIntent()
        }
    }

    private fun signInSilently() {
        _logger.debug("signInSilently")
        val activity = _activity ?: return
        val client = _signInClient ?: return
        client.silentSignIn().addOnCompleteListener(activity) { task ->
            if (task.isSuccessful) {
                _logger.debug("signInSilently: success")
            } else {
                _logger.debug("signInSilently: failed")
            }
        }
    }

    private fun startSignInIntent() {
        _logger.debug("startSignInIntent")
        val activity = _activity ?: return
        val client = _signInClient ?: return
        activity.startActivityForResult(client.signInIntent, RC_SIGN_IN)
    }

    private fun signOut() {
        val activity = _activity ?: return
        val client = _signInClient ?: return
        client.signOut().addOnCompleteListener(activity) {
            _logger.debug("signOut(): success")
        }
    }

    private fun showAchievements() {
        if (!isSignedIn()) {
            return
        }
        achievementsClient?.achievementsIntent?.addOnSuccessListener { intent ->
            val activity = _activity ?: return@addOnSuccessListener
            activity.startActivityForResult(intent, RC_ACHIEVEMENT_UI)
        }
    }

    // Load all achievements and check increment
    // Ref: https://stackoverflow.com/questions/23848014/google-play-game-services-unlock
    // -achievement-store-unlock-in-game-or-call-unlo/23853222#23853222
    private fun incrementAchievement(achievementId: String?, percent: Double) {
        if (!isSignedIn()) {
            return
        }
        val client = achievementsClient
        client?.load(true)?.addOnCompleteListener { task ->
            val buff = task.result!!.get()
            val bufSize = buff!!.count
            for (i in 0 until bufSize) {
                val ach = buff[i]
                val id = ach.achievementId
                if (ach.type != Achievement.TYPE_INCREMENTAL) {
                    continue
                }
                if (id.compareTo(achievementId!!) == 0) {
                    val currStep = ach.currentSteps
                    val totalStep = ach.totalSteps
                    val reportStep = (percent * totalStep).toInt()
                    val increment = reportStep - currStep
                    if (increment > 0) {
                        client.increment(achievementId, increment)
                    }
                    break
                }
            }
            buff.release()
        }
    }

    private fun unlockAchievement(achievementId: String?) {
        if (!isSignedIn()) {
            return
        }
        achievementsClient?.unlock(achievementId!!)
    }

    private fun showLeaderboard(leaderboardId: String) {
        if (!isSignedIn()) {
            return
        }
        leaderboardsClient?.getLeaderboardIntent(leaderboardId)?.addOnSuccessListener { intent ->
            val activity = _activity ?: return@addOnSuccessListener
            activity.startActivityForResult(intent, RC_LEADER_BOARD_UI)
        }
    }

    private fun showAllLeaderboards() {
        if (!isSignedIn()) {
            return
        }
        leaderboardsClient?.allLeaderboardsIntent?.addOnSuccessListener { intent ->
            val activity = _activity ?: return@addOnSuccessListener
            activity.startActivityForResult(intent, RC_LEADER_BOARD_UI)
        }
    }

    private fun submitScore(leaderboardName: String, score: Long) {
        if (!isSignedIn()) {
            return
        }
        leaderboardsClient?.submitScore(leaderboardName, score)
    }

    private val achievementsClient: AchievementsClient?
        get() {
            val activity = _activity ?: return null
            val account = GoogleSignIn.getLastSignedInAccount(activity) ?: return null
            return Games.getAchievementsClient(activity, account)
        }

    private val leaderboardsClient: LeaderboardsClient?
        get() {
            val activity = _activity ?: return null
            val account = GoogleSignIn.getLastSignedInAccount(activity) ?: return null
            return Games.getLeaderboardsClient(activity, account)
        }
}