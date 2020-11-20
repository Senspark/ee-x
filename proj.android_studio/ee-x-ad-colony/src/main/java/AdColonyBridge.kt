import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.adcolony.sdk.AdColony
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.IPlugin
import com.ee.Thread
import com.ee.Utils
import kotlinx.serialization.InternalSerializationApi
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

@InternalSerializationApi
class AdColonyBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = AdColonyBridge::class.java.name
        private const val kPrefix = "AdColonyBridge"
        private const val kInitialize = "${kPrefix}Initialize"
    }

    private var _initializing = false
    private var _initialized = false

    init {
        _logger.info("$kTag: constructor begin: context = $_context")
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

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            Utils.toString(initialize(message))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }

    @AnyThread
    suspend fun initialize(appId: String): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                if (_initializing) {
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (_initialized) {
                    cont.resume(true)
                    return@runOnMainThread
                }
                _initializing = true
                AdColony.configure(_activity, appId);
                _initializing = false
                _initialized = true
                cont.resume(true)
            }
        }
    }
}