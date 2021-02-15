import {
    FullScreenAdResult,
    IFullScreenAd,
} from "./IFullScreenAd";
import { NullAd } from "./NullAd";

export class NullFullScreenAd extends NullAd implements IFullScreenAd {
    public async show(): Promise<FullScreenAdResult> {
        return FullScreenAdResult.Failed;
    }
}