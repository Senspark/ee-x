package com.ee

import android.app.Activity
import com.google.android.gms.games.PlayGames
import com.google.android.gms.games.SnapshotsClient
import com.google.android.gms.games.snapshot.SnapshotMetadataChange

class CloudSave() {
    private lateinit var _activity: Activity
    private lateinit var _logger: ILogger

    companion object {
        private const val K_SAVE_NAME = "save_data"
    }

    fun onCreate(activity: Activity, logger: ILogger) {
        _activity = activity
        _logger = logger
    }

    fun push(saveData: String, completion: (Boolean) -> Unit) {
        val client = PlayGames.getSnapshotsClient(_activity)

        client.open(K_SAVE_NAME, true, SnapshotsClient.RESOLUTION_POLICY_MOST_RECENTLY_MODIFIED)
            .addOnSuccessListener { task ->
                if (task.isConflict) {
                    _logger.info("snapshot isConflict")
                    return@addOnSuccessListener
                }
                val snapshot = task.data
                if (snapshot == null) {
                    _logger.info("snapshot is null")
                    return@addOnSuccessListener
                }

                val bytes = saveData.toByteArray()
                snapshot.snapshotContents.writeBytes(bytes)

                val snapshotMetadataChange = SnapshotMetadataChange.Builder()
                    .setDescription("Game save data")
                    .build()
                client.commitAndClose(snapshot, snapshotMetadataChange)
                    .addOnSuccessListener {
                        _logger.info("snapshot commit success")
                        completion(true)
                    }
                    .addOnFailureListener {
                        _logger.info("snapshot commit failed")
                        completion(false)
                    }
                    .addOnCanceledListener {
                        _logger.info("snapshot commit canceled")
                        completion(false)
                    }
            }
            .addOnFailureListener {
                _logger.info("snapshot open failed")
                completion(false)
            }
            .addOnCanceledListener {
                _logger.info("snapshot open canceled")
                completion(false)
            }
    }

    fun pull(completion: (String) -> Unit) {
        val client = PlayGames.getSnapshotsClient(_activity)
        client.open(K_SAVE_NAME, true, SnapshotsClient.RESOLUTION_POLICY_MOST_RECENTLY_MODIFIED)
            .addOnSuccessListener { task ->
                if (task.isConflict) {
                    _logger.info("snapshot isConflict")
                    return@addOnSuccessListener
                }
                val snapshot = task.data
                if (snapshot == null) {
                    _logger.info("snapshot is null")
                    return@addOnSuccessListener
                }

                val bytes = snapshot.snapshotContents.readFully()
                val saveData = String(bytes)
                completion(saveData)
            }
            .addOnFailureListener {
                _logger.info("snapshot open failed")
                completion("")
            }
            .addOnCanceledListener {
                _logger.info("snapshot open canceled")
                completion("")
            }
    }
}