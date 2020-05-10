---
classes: wide
---
- Modify `Android.mk`:
{% highlight make %}
$(call import-add-path, CLONE_PATH)
$(call import-module, ee-x)
{% endhighlight %}

- Modify `main.cpp`:
{% highlight cpp %}
#include <jni.h>
#include <ee/Core.hpp>

void cocos_android_app_init(JNIEnv* env) {
    ...
    JavaVM* vm;
    env->GetJavaVm(&vm);
    ee::JniUtils::setVm(vm);
}
{% endhighlight %}

- Modify `settings.gradle`:
{% highlight gradle %}
include ':ee-x-core'
project(':ee-x-core').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-core')
{% endhighlight %}

- Add or modify `MyApplication.java`:
{% highlight java %}
import com.ee.core.PluginManager;

public MyApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        PluginManager.getInstance().initializePlugins(this);
    }
}
{% endhighlight %}

- Modify `AppActivity.java`:
{% highlight java %}
import com.ee.core.PluginManager;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        PluginManager.getInstance().onCreate(this);
    }

    @Override
    protected void onStart() {
        super.onStart();
        PluginManager.getInstance().onStart();
    }
    
    @Override
    protected void onStop() {
        super.onStop();
        PluginManager.getInstance().onStop();
    }

    @Override
    protected void onResume() {
        super.onResume();
        PluginManager.getInstance().onResume();
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        PluginManager.getInstance().onPause();
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        PluginManager.getInstance().onDestroy();
    }
    
    @Override
    public void onBackPressed() {
        if (PluginManager.getInstance().onBackPressed()) {
            return;
        }
        super.onBackPressed();
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (PluginManager.getInstance().onActivityResult(requestCode, resultCode, data)) {
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }
}
{% endhighlight %}

- Modify `AppController.mm`:
{% highlight objc %}
#import <ee/Core.hpp>

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    ...
    [[EEPluginManager getInstance] initializePlugins];
    return YES;
}

{% endhighlight %}