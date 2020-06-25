//
//  Tenjin.swift
//  ee-x-df17f84c
//
//  Created by eps on 6/4/20.
//

import Foundation

@objc(EETenjinBridge)
public class TenjinBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge

    public required init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
    }

    public func destroy() {}
}
