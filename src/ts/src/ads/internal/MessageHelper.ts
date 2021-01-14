export class MessageHelper {
    private readonly _prefix: string;
    private readonly _adId: string;

    public constructor(prefix: string, adId: string) {
        this._prefix = prefix;
        this._adId = adId;
    }

    public get isLoaded(): string {
        return `${this._prefix}_isLoaded_${this._adId}`;
    }

    public get load(): string {
        return `${this._prefix}_load_${this._adId}`;
    }

    public get onLoaded(): string {
        return `${this._prefix}_onLoaded_${this._adId}`;
    }

    public get onFailedToLoad(): string {
        return `${this._prefix}_onFailedToLoad_${this._adId}`;
    }

    public get show(): string {
        return `${this._prefix}_show_${this._adId}`;
    }

    public get onFailedToShow(): string {
        return `${this._prefix}_onFailedToShow_${this._adId}`;
    }

    public get onClicked(): string {
        return `${this._prefix}_onClicked_${this._adId}`;
    }

    public get onClosed(): string {
        return `${this._prefix}_onClosed_${this._adId}`;
    }

    public get getPosition(): string {
        return `${this._prefix}_getPosition_${this._adId}`;
    }

    public get setPosition(): string {
        return `${this._prefix}_setPosition_${this._adId}`;
    }

    public get getSize(): string {
        return `${this._prefix}_getSize_${this._adId}`;
    }

    public get setSize(): string {
        return `${this._prefix}_setSize_${this._adId}`;
    }

    public get isVisible(): string {
        return `${this._prefix}_isVisible_${this._adId}`;
    }

    public get setVisible(): string {
        return `${this._prefix}_setVisible_${this._adId}`;
    }
}