//
//  FirebaseCoreBridge.swift
//  Pods
//
//  Created by eps on 6/26/20.
//

@objc(EEFirebaseCoreBridge)
public class FirebaseCoreBridge: NSObject, IPlugin {
    public required init(_ bridge: IMessageBridge) {
        super.init()
        FirebaseApp.configure()
    }

    public func destroy() {}
}
