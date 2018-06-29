package com.ee.googleplay;

import android.app.Activity;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

import java.util.Map;

public class GooglePlay implements PluginProtocol {

    // Define
    private static final String k_isSignedIn            = "GooglePlay_isSignedIn";
    private static final String k_signin                = "GooglePlay_signin";
    private static final String k_signout               = "GooglePlay_signout";
    private static final String k_showAchievements      = "GooglePlay_showAchievements";
    private static final String k_increaseAchievement   = "GooglePlay_increaseAchievement";
    private static final String k_unlockAchievement     = "GooglePlay_unlockAchievement";
    private static final String k_showLeaderboard       = "GooglePlay_showLeaderboard";
    private static final String k_showAllLeaderboards   = "GooglePlay_showAllLeaderboards";
    private static final String k_submitScore           = "GooglePlay_submitScore";

    private static final String k_showLoginUI           = "show_login_ui";
    private static final String k_achievementId         = "achievement_id";
    private static final String k_increment             = "increment";
    private static final String k_leaderboardId          = "leaderboard_id";
    private static final String k_score                 = "score";

    private static final int RC_SIGN_IN = 9001;
    private static final int RC_ACHIEVEMENT_UI = 9002;
    private static final int RC_LEADERBOARD_UI = 9003;
    private static final Logger _logger = new Logger(GooglePlay.class.getName());

    private Activity _activity;

    // Google signin client
    private GoogleSignInClient _signinClient = null;
    // Other client
    private AchievementsClient _achievementClient = null;
    private LeaderboardsClient _leaderboardClient = null;

    private boolean _pendingShowAchievement = false;
    private boolean _pendingShowLeaderboard = false;
    private String _lastLeaderboardId = "";

    public GooglePlay() {
        Utils.checkMainThread();
        _activity = null;
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "GooglePlay";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;

        // Init client
        _signinClient = GoogleSignIn.getClient(_activity, new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN).build());
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                boolean isSignedIn = isSignedIn();
                return Utils.toString(isSignedIn);
            }
        }, k_isSignedIn);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                boolean showLoginUi = (boolean)dict.get(k_showLoginUI);
                signin(showLoginUi);
                return null;
            }
        }, k_signin);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                signout();
                return null;
            }
        }, k_signout);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showAchievements();
                return null;
            }
        }, k_showAchievements);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String achievementId = (String)dict.get(k_achievementId);
                Integer increment = (Integer)dict.get(k_increment);
                incrementAchievement(achievementId, increment);

                return null;
            }
        }, k_increaseAchievement);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String achievementId = (String)dict.get(k_achievementId);
                unlockAchievement(achievementId);

                return null;
            }
        }, k_unlockAchievement);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String leaderboardId = (String)dict.get(k_leaderboardId);
                showLeaderboard(leaderboardId);

                return null;
            }
        }, k_showLeaderboard);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showAllLeaderboards();
                return null;
            }
        }, k_showAllLeaderboards);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String leaderboardId = (String)dict.get(k_leaderboardId);
                Long score = (Long)dict.get(k_score);
                submitScore(leaderboardId, score);
                return null;
            }
        }, k_submitScore);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k_isSignedIn);
        bridge.deregisterHandler(k_signin);
        bridge.deregisterHandler(k_signout);
        bridge.deregisterHandler(k_showAchievements);
        bridge.deregisterHandler(k_increaseAchievement);
        bridge.deregisterHandler(k_unlockAchievement);
        bridge.deregisterHandler(k_showLeaderboard);
        bridge.deregisterHandler(k_showAllLeaderboards);
        bridge.deregisterHandler(k_submitScore);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        if (requestCode == RC_SIGN_IN)
        {
            onSigninCallback(responseCode == Activity.RESULT_OK);
        }
        return true;
    }


    private boolean isSignedIn()
    {
        if (_activity == null)
        {
            return false;
        }

        return GoogleSignIn.getLastSignedInAccount(_activity) != null;
    }

    private void signin(boolean showLoginUI)
    {
        if (showLoginUI)
        {
            this.startSignInIntent();
        }
        else
        {
            signInSilently();
        }
    }

    private void signInSilently() {
        _logger.debug("signInSliently");

        _signinClient.silentSignIn().addOnCompleteListener(_activity,
                new OnCompleteListener<GoogleSignInAccount>() {
                    @Override
                    public void onComplete(@NonNull Task<GoogleSignInAccount> task) {
                        if (task.isSuccessful()) {
                            _logger.debug("signInSliently: success");
                        } else {
                            _logger.debug("signInSliently: failed");
                        }

                        // TODO: Handle callback
                    }
                });
    }

    private void startSignInIntent() {
        _activity.startActivityForResult(_signinClient.getSignInIntent(), RC_SIGN_IN);
    }

    private void signout()
    {
        _signinClient.signOut().addOnCompleteListener(_activity,
                new OnCompleteListener<Void>() {
                    @Override
                    public void onComplete(@NonNull Task<Void> task) {
                        boolean successful = task.isSuccessful();
                        _logger.debug("signOut(): " + (successful ? "success" : "failed"));
                        //TODO: Handle signout
                    }
                });
    }

    private void onSigninCallback(boolean suc)
    {
        if (suc)
        {
            if (_pendingShowAchievement)
            {
                showAchievements();
            }
            else if (_pendingShowLeaderboard)
            {
                if (_lastLeaderboardId.compareTo("") == 0)
                {
                    showAllLeaderboards();
                }
                else
                {
                    showLeaderboard(_lastLeaderboardId);
                }
            }
        }

        _pendingShowAchievement = false;
        _pendingShowLeaderboard = false;
    }

    private void showAchievements()
    {
        if (!checkSigninAndAchievementClient())
        {
            return;
        }
        if (_achievementClient != null)
        {
            _achievementClient.getAchievementsIntent().addOnSuccessListener(new OnSuccessListener<Intent>() {
                @Override
                public void onSuccess(Intent intent) {
                    _activity.startActivityForResult(intent, RC_ACHIEVEMENT_UI);
                }
            });
        }
    }

    private void incrementAchievement(String achievement_name, int increment)
    {
        if (!isSignedIn())
        {
            return;
        }

        if (_achievementClient != null)
        {
            _achievementClient.increment(achievement_name, increment);
        }
    }

    private void unlockAchievement(String achievement_name )
    {
        if (!isSignedIn())
        {
            return;
        }
        if (_achievementClient != null)
        {
            _achievementClient.unlock(achievement_name);
        }
    }

    public boolean checkSigninAndAchievementClient()
    {
        if (_activity == null)
        {
            return false;
        }

        if (!isSignedIn())
        {
            signin(false);
            return false;
        }

        if (_achievementClient == null)
        {
            GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
            if (account == null)
            {
                return false;
            }
            _achievementClient = Games.getAchievementsClient(_activity, account);
        }

        return _achievementClient != null;
    }

    private void showLeaderboard(String leaderboard_name)
    {
        if (!checkSigninAndLeaderboardClient(leaderboard_name))
        {
            return;
        }

        if (_leaderboardClient != null)
        {
            _leaderboardClient.getLeaderboardIntent(leaderboard_name).addOnSuccessListener(new OnSuccessListener<Intent>() {
                @Override
                public void onSuccess(Intent intent) {
                    _activity.startActivityForResult(intent, RC_LEADERBOARD_UI);
                }
            });
        }
    }

    private void showAllLeaderboards()
    {
        if (!checkSigninAndLeaderboardClient(""))
        {
            return;
        }

        if (_leaderboardClient != null)
        {
            _leaderboardClient.getAllLeaderboardsIntent().addOnSuccessListener(new OnSuccessListener<Intent>() {
                @Override
                public void onSuccess(Intent intent) {
                    _activity.startActivityForResult(intent, RC_LEADERBOARD_UI);
                }
            });
        }
    }

    private void submitScore(String leaderboard_name, long score)
    {
        if (!isSignedIn())
        {
            return;
        }
        GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
        if (account == null)
        {
            return;
        }

        Games.getLeaderboardsClient(_activity, account).submitScore(leaderboard_name, score);
    }

    public boolean checkSigninAndLeaderboardClient(String saveLeaderboardId)
    {
        if (_activity == null)
        {
            return false;
        }

        if (!isSignedIn())
        {
            signin(false);
            return false;
        }

        if (_achievementClient == null)
        {
            GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
            if (account == null)
            {
                return false;
            }
            _leaderboardClient = Games.getLeaderboardsClient(_activity, account);
        }

        _lastLeaderboardId = saveLeaderboardId;
        return _leaderboardClient != null;
    }
}
