//
//  Tenjin.swift
//  ee-x-df17f84c
//
//  Created by eps on 6/4/20.
//

import Foundation

@objc(EETenjin)
public class Tenjin: NSObject {
    private let _bridge: IMessageBridge

    @objc
    public init(_ bridge: IMessageBridge) {
        _bridge = bridge
        super.init()
    }
}
