export class NotificationBuilder {
    private _ticker: string;
    private _title: string;
    private _body: string;
    private _delay: number;
    private _interval: number;
    private _tag: number;

    public constructor() {
        this._ticker = ``;
        this._title = ``;
        this._body = ``;
        this._delay = 0;
        this._interval = 0;
        this._tag = 0;
    }

    public get ticker(): string {
        return this._ticker;
    }

    public set ticker(value: string) {
        this._ticker = value;
    }

    public get title(): string {
        return this._title;
    }

    public set title(value: string) {
        this._title = value;
    }

    public get body(): string {
        return this._body;
    }

    public set body(value: string) {
        this._body = value;
    }

    public get delay(): number {
        return this._delay;
    }

    public set delay(value: number) {
        this._delay = value;
    }

    public get interval(): number {
        return this._interval;
    }

    public set interval(value: number) {
        this._interval = value;
    }

    public get tag(): number {
        return this._tag;
    }

    public set tag(value: number) {
        this._tag = value;
    }
}