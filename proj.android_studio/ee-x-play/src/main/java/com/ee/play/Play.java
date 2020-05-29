package com.ee.play;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.AnnotatedData;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

import java.util.Map;

public class Play implements PluginProtocol {
    private static final String k_isSignedIn = "Play_isSignedIn";
    private static final String k_signin = "Play_signin";
    private static final String k_signout = "Play_signout";
    private static final String k_showAchievements = "Play_showAchievements";
    private static final String k_increaseAchievement = "Play_increaseAchievement";
    private static final String k_unlockAchievement = "Play_unlockAchievement";
    private static final String k_showLeaderboard = "Play_showLeaderboard";
    private static final String k_showAllLeaderboards = "Play_showAllLeaderboards";
    private static final String k_submitScore = "Play_submitScore";
    private static final String k_onSignedIn = "Play_onSignedIn";

    private static final String k_silent_sign_in = "silent_sign_in";
    private static final String k_achievementId = "achievement_id";
    private static final String k_increment = "increment";
    private static final String k_leaderboardId = "leaderboard_id";
    private static final String k_score = "score";

    private static final int RC_SIGN_IN = 201912001;
    private static final int RC_ACHIEVEMENT_UI = 201912002;
    private static final int RC_LEADER_BOARD_UI = 201912003;

    private static final Logger _logger = new Logger(Play.class.getName());

    private Activity _activity;
    private IMessageBridge _bridge;
    private GoogleSignInClient _signinClient = null;
    private GoogleSignInOptions _signInOptions = null;

    public Play(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Utils.checkMainThread();
        _bridge = bridge;
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Play";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
        _signInOptions = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN;
        _signinClient = GoogleSignIn.getClient(_activity, _signInOptions);
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
        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                boolean isSignedIn = isSignedIn();
                return Utils.toString(isSignedIn);
            }
        }, k_isSignedIn);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                boolean silentSignIn = (boolean) dict.get(k_silent_sign_in);
                signIn(silentSignIn);
                return "";
            }
        }, k_signin);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                signOut();
                return "";
            }
        }, k_signout);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showAchievements();
                return "";
            }
        }, k_showAchievements);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String achievementId = (String) dict.get(k_achievementId);
                double increment = (double) dict.get(k_increment);
                incrementAchievement(achievementId, increment);
                return "";
            }
        }, k_increaseAchievement);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String achievementId = (String) dict.get(k_achievementId);
                unlockAchievement(achievementId);
                return "";
            }
        }, k_unlockAchievement);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String leaderboardId = (String) dict.get(k_leaderboardId);
                showLeaderboard(leaderboardId);
                return "";
            }
        }, k_showLeaderboard);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showAllLeaderboards();
                return "";
            }
        }, k_showAllLeaderboards);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String leaderboardId = (String) dict.get(k_leaderboardId);
                Long score = Long.valueOf(dict.get(k_score).toString());
                submitScore(leaderboardId, score);
                return "";
            }
        }, k_submitScore);
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k_isSignedIn);
        _bridge.deregisterHandler(k_signin);
        _bridge.deregisterHandler(k_signout);
        _bridge.deregisterHandler(k_showAchievements);
        _bridge.deregisterHandler(k_increaseAchievement);
        _bridge.deregisterHandler(k_unlockAchievement);
        _bridge.deregisterHandler(k_showLeaderboard);
        _bridge.deregisterHandler(k_showAllLeaderboards);
        _bridge.deregisterHandler(k_submitScore);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        if (requestCode == RC_SIGN_IN) {
            boolean success = responseCode == Activity.RESULT_OK;
            _bridge.callCpp(k_onSignedIn, Utils.toString(success));
            return true;
        }
        return false;
    }

    // Check whether player is already signed in
    // https://developers.google.com/games/services/android/signin
    private boolean isSignedIn() {
        if (_activity == null) {
            return false;
        }
        GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
        if (account != null && GoogleSignIn.hasPermissions(account,
            _signInOptions.getScopeArray())) {
            // Already signed in.
            return true;
        } else {
            // Haven't been signed-in before
            return false;
        }

    }

    private void signIn(boolean silentSignIn) {
        _logger.debug("signIn " + silentSignIn);
        if (silentSignIn) {
            signInSilently();
        } else {
            startSignInIntent();
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
                }
            });
    }

    private void startSignInIntent() {
        _logger.debug("startSignInIntent");
        _activity.startActivityForResult(_signinClient.getSignInIntent(), RC_SIGN_IN);
    }

    private void signOut() {
        _signinClient.signOut().addOnCompleteListener(_activity, new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                _logger.debug("signOut(): success");
            }
        });
    }

    private void showAchievements() {
        if (!isSignedIn()) {
            return;
        }
        getAchievementsClient().getAchievementsIntent().addOnSuccessListener(new OnSuccessListener<Intent>() {
            @Override
            public void onSuccess(Intent intent) {
                _activity.startActivityForResult(intent, RC_ACHIEVEMENT_UI);
            }
        });
    }

    // Load all achievements and check increment
    // Ref: https://stackoverflow.com/questions/23848014/google-play-game-services-unlock
    // -achievement-store-unlock-in-game-or-call-unlo/23853222#23853222
    private void incrementAchievement(final String achievementId, final double percent) {
        if (!isSignedIn()) {
            return;
        }
        final AchievementsClient client = getAchievementsClient();
        client.load(true).addOnCompleteListener(new OnCompleteListener<AnnotatedData<AchievementBuffer>>() {
            @Override
            public void onComplete(@NonNull Task<AnnotatedData<AchievementBuffer>> task) {
                AchievementBuffer buff = task.getResult().get();
                int bufSize = buff.getCount();
                for (int i = 0; i < bufSize; i++) {
                    Achievement ach = buff.get(i);
                    String id = ach.getAchievementId();
                    if (ach.getType() != Achievement.TYPE_INCREMENTAL) {
                        continue;
                    }
                    if (id.compareTo(achievementId) == 0) {
                        int currStep = ach.getCurrentSteps();
                        int totalStep = ach.getTotalSteps();
                        int reportStep = (int) (percent * totalStep);
                        int increment = reportStep - currStep;
                        if (increment > 0) {
                            client.increment(achievementId, increment);
                        }
                        break;
                    }
                }
                buff.release();
            }
        });
    }

    private void unlockAchievement(String achievementId) {
        if (!isSignedIn()) {
            return;
        }
        getAchievementsClient().unlock(achievementId);

    }

    private void showLeaderboard(String leaderBoardId) {
        if (!isSignedIn()) {
            return;
        }
        getLeaderboardsClient().getLeaderboardIntent(leaderBoardId).addOnSuccessListener(new OnSuccessListener<Intent>() {
            @Override
            public void onSuccess(Intent intent) {
                _activity.startActivityForResult(intent, RC_LEADER_BOARD_UI);
            }
        });
    }

    private void showAllLeaderboards() {
        if (!isSignedIn()) {
            return;
        }
        getLeaderboardsClient().getAllLeaderboardsIntent().addOnSuccessListener(new OnSuccessListener<Intent>() {
            @Override
            public void onSuccess(Intent intent) {
                _activity.startActivityForResult(intent, RC_LEADER_BOARD_UI);
            }
        });
    }

    private void submitScore(String leaderboardName, long score) {
        if (!isSignedIn()) {
            return;
        }
        getLeaderboardsClient().submitScore(leaderboardName, score);
    }

    private AchievementsClient getAchievementsClient() {
        return Games.getAchievementsClient(_activity,
            GoogleSignIn.getLastSignedInAccount(_activity));
    }

    private LeaderboardsClient getLeaderboardsClient() {
        return Games.getLeaderboardsClient(_activity,
            GoogleSignIn.getLastSignedInAccount(_activity));
    }
}
