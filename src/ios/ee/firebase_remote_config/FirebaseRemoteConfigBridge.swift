//
//  FirebaseRemoteConfigBridge.swift
//  ee-x-a16b5188
//
//  Created by eps on 2/2/21.
//

private let kPrefix = "FirebaseRemoteConfigBridge"
private let kSetSettings = "\(kPrefix)SetSettings"
private let kFetch = "\(kPrefix)Fetch"
private let kActivate = "\(kPrefix)Activate"
private let kGetBool = "\(kPrefix)GetBool"
private let kGetLong = "\(kPrefix)GetLong"
private let kGetDouble = "\(kPrefix)GetDouble"
private let kGetString = "\(kPrefix)GetString"
private let kSetDefaults = "\(kPrefix)SetDefaults"

@objc(EEFirebaseRemoteConfigBridge)
class FirebaseRemoteConfigBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    
    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        super.init()
    }
}
