package com.ee.game;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Build;
import androidx.annotation.NonNull;

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
import com.google.android.gms.games.AnnotatedData;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

import java.util.Map;

public class GameCenter implements PluginProtocol {
    //TODO: Need to cache achievements in client, if user signed in then report them to store
    //TODO: Need to optimize when report achievement into score, don't need to load all of them

    // Define
    private static final String k_isSignedIn          = "GameCenter_isSignedIn";
    private static final String k_signin              = "GameCenter_signin";
    private static final String k_signout             = "GameCenter_signout";
    private static final String k_showAchievements    = "GameCenter_showAchievements";
    private static final String k_increaseAchievement = "GameCenter_increaseAchievement";
    private static final String k_unlockAchievement   = "GameCenter_unlockAchievement";
    private static final String k_showLeaderboard     = "GameCenter_showLeaderboard";
    private static final String k_showAllLeaderboards = "GameCenter_showAllLeaderboards";
    private static final String k_submitScore         = "GameCenter_submitScore";

    private static final String k_showLoginUI   = "show_login_ui";
    private static final String k_achievementId = "achievement_id";
    private static final String k_increment     = "increment";
    private static final String k_leaderboardId = "leaderboard_id";
    private static final String k_score         = "score";

    private static final int    RC_SIGN_IN        = 2921001;
    private static final int    RC_ACHIEVEMENT_UI = 2921002;
    private static final int    RC_LEADERBOARD_UI = 2921003;
    private static final Logger _logger           = new Logger(GameCenter.class.getName());

    private Activity _activity;

    // Google signin client
    private GoogleSignInClient _signinClient      = null;
    // Other client
    private AchievementsClient _achievementClient = null;
    private LeaderboardsClient _leaderboardClient = null;

    private boolean _pendingShowAchievement = false;
    private boolean _pendingShowLeaderboard = false;
    private String  _lastLeaderboardId      = "";

    public GameCenter() {
        Thread.checkMainThread();
        _activity = null;
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "GameCenter";
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
        Thread.checkMainThread();
        deregisterHandlers();
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
                boolean showLoginUi = (boolean) dict.get(k_showLoginUI);
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
                String achievementId = (String) dict.get(k_achievementId);
                double increment = (double) dict.get(k_increment);
                incrementAchievement(achievementId, increment);

                return null;
            }
        }, k_increaseAchievement);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String achievementId = (String) dict.get(k_achievementId);
                unlockAchievement(achievementId);

                return null;
            }
        }, k_unlockAchievement);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String leaderboardId = (String) dict.get(k_leaderboardId);
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
                String leaderboardId = (String) dict.get(k_leaderboardId);
                Long score = (Long) dict.get(k_score);
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
        if (requestCode == RC_SIGN_IN) {
            onSigninCallback(responseCode == Activity.RESULT_OK);
            return true;
        }
        return false;
    }


    private boolean isSignedIn() {
        if (_activity == null) {
            return false;
        }

        return GoogleSignIn.getLastSignedInAccount(_activity) != null;
    }

    private void signin(boolean showLoginUI) {
        if (showLoginUI) {
            this.startSignInIntent();
        } else {
            signInSilently();
        }
    }

    private void signInSilently() {
        _logger.debug("signInSliently");

        _signinClient.silentSignIn().addOnCompleteListener(_activity, new OnCompleteListener<GoogleSignInAccount>() {
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
        _activity.startActivityForResult(_signinClient.getSignInIntent(), RC_SIGN_IN);
    }

    private void signout() {
        _signinClient.signOut().addOnCompleteListener(_activity, new OnCompleteListener<Void>() {
            @Override
            public void onComplete(@NonNull Task<Void> task) {
                boolean successful = task.isSuccessful();
                _logger.debug("signOut(): " + (successful ? "success" : "failed"));
            }
        });
    }

    private void onSigninCallback(boolean suc) {
        if (suc) {
            if (_pendingShowAchievement) {
                showAchievements();
            } else if (_pendingShowLeaderboard) {
                if (_lastLeaderboardId.compareTo("") == 0) {
                    showAllLeaderboards();
                } else {
                    showLeaderboard(_lastLeaderboardId);
                }
            }
        } else {
            _activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    AlertDialog.Builder builder;
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        builder = new AlertDialog.Builder(_activity, android.R.style.Theme_Material_Dialog_Alert);
                    } else {
                        builder = new AlertDialog.Builder(_activity);
                    }
                    builder.setMessage("Failed to sign in. Please check your network connection and try again.").setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            // continue with delete
                            dialog.dismiss();
                        }
                    }).show();
                }
            });
        }

        _pendingShowAchievement = false;
        _pendingShowLeaderboard = false;
    }

    private void showAchievements() {
        if (!checkSigninAndAchievementClient()) {
            return;
        }
        if (_achievementClient != null) {
            _achievementClient.getAchievementsIntent().addOnSuccessListener(new OnSuccessListener<Intent>() {
                @Override
                public void onSuccess(Intent intent) {
                    _activity.startActivityForResult(intent, RC_ACHIEVEMENT_UI);
                }
            });
        }
    }

    // Load all achievements and check increment
    // Ref: https://stackoverflow.com/questions/23848014/google-play-game-services-unlock-achievement-store-unlock-in-game-or-call-unlo/23853222#23853222
    // ==========
    private void incrementAchievement(final String achievement_id, final double percent) {
        if (!isSignedIn()) {
            return;
        }

        if (_achievementClient != null) {
            _achievementClient.load(true).addOnCompleteListener(new OnCompleteListener<AnnotatedData<AchievementBuffer>>() {
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
                        if (id.compareTo(achievement_id) == 0) {
                            int currStep = ach.getCurrentSteps();
                            int totalStep = ach.getTotalSteps();
                            int reportStep = (int) (percent * totalStep);
                            int increment = reportStep - currStep;
                            if (increment > 0) {
                                _achievementClient.increment(achievement_id, increment);
                            }
                            break;
                        }
                    }
                    buff.release();
                }
            });
        }
    }

    private void unlockAchievement(String achievement_id) {
        if (!isSignedIn()) {
            return;
        }
        if (_achievementClient != null) {
            _achievementClient.unlock(achievement_id);
        }
    }

    public boolean checkSigninAndAchievementClient() {
        if (_activity == null) {
            return false;
        }

        if (!isSignedIn()) {
            signin(false);
            return false;
        }

        if (_achievementClient == null) {
            GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
            if (account == null) {
                return false;
            }
            _achievementClient = Games.getAchievementsClient(_activity, account);
        }

        return _achievementClient != null;
    }

    private void showLeaderboard(String leaderboard_name) {
        if (!checkSigninAndLeaderboardClient(leaderboard_name)) {
            return;
        }

        if (_leaderboardClient != null) {
            _leaderboardClient.getLeaderboardIntent(leaderboard_name).addOnSuccessListener(new OnSuccessListener<Intent>() {
                @Override
                public void onSuccess(Intent intent) {
                    _activity.startActivityForResult(intent, RC_LEADERBOARD_UI);
                }
            });
        }
    }

    private void showAllLeaderboards() {
        if (!checkSigninAndLeaderboardClient("")) {
            return;
        }

        if (_leaderboardClient != null) {
            _leaderboardClient.getAllLeaderboardsIntent().addOnSuccessListener(new OnSuccessListener<Intent>() {
                @Override
                public void onSuccess(Intent intent) {
                    _activity.startActivityForResult(intent, RC_LEADERBOARD_UI);
                }
            });
        }
    }

    private void submitScore(String leaderboard_name, long score) {
        if (!isSignedIn()) {
            return;
        }
        GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
        if (account == null) {
            return;
        }

        Games.getLeaderboardsClient(_activity, account).submitScore(leaderboard_name, score);
    }

    public boolean checkSigninAndLeaderboardClient(String saveLeaderboardId) {
        if (_activity == null) {
            return false;
        }

        if (!isSignedIn()) {
            signin(false);
            return false;
        }

        if (_achievementClient == null) {
            GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(_activity);
            if (account == null) {
                return false;
            }
            _leaderboardClient = Games.getLeaderboardsClient(_activity, account);
        }

        _lastLeaderboardId = saveLeaderboardId;
        return _leaderboardClient != null;
    }
}
