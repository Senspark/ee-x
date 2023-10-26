//
//  AdPaidResponse.swift
//  ee-x
//
//  Created by Nhan on 11/09/2023.
//

class AdPaidResponse {
    let adUnitId: String
    let adFormat: String
    let revenue: NSDecimalNumber
    let networkName: String
    
    init(adUnitId: String, adFormat: String, revenue: NSDecimalNumber, networkName: String) {
        self.adUnitId = adUnitId
        self.adFormat = adFormat
        self.revenue = revenue
        self.networkName = networkName
    }
}

struct AdRevenueData : Codable {
    let networkName: String
    let mediationName: String
    let adUnitId: String
    let adFormat: String
    let revenue: Double
}
