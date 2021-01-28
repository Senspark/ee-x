export class FacebookGraphRequest {
    private _path: string;
    private readonly _parameters: { [key: string]: string };

    public constructor() {
        this._path = ``;
        this._parameters = {};
    }

    public setPath(path: string): this {
        this._path = path;
        return this;
    }

    public setParameter(key: string, value: string): this {
        this._parameters[key] = value;
        return this;
    }

    public toString(): string {
        return JSON.stringify({
            path: this._path,
            parameters: this._parameters,
        });
    }
}