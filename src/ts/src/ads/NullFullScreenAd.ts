import {
    AdResult,
    IFullScreenAd,
} from "./IFullScreenAd";
import { NullAd } from "./NullAd";

export class NullFullScreenAd extends NullAd implements IFullScreenAd {
    public async show(): Promise<AdResult> {
        return AdResult.Failed;
    }
}