//
//  AdPaidResponse.swift
//  ee-x
//
//  Created by Nhan on 11/09/2023.
//

class AdPaidResponse {
    let adUnitId: String
    let adFormat: String
    let valueMicros: NSDecimalNumber
    let networkName: String
    
    init(adUnitId: String, adFormat: String, valueMicros: NSDecimalNumber, networkName: String) {
        self.adUnitId = adUnitId
        self.adFormat = adFormat
        self.valueMicros = valueMicros
        self.networkName = networkName
    }
}

