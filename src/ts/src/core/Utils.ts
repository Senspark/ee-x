import {ILogger} from "./ILogger";

export class Utils {
    public static toString(value: boolean): string {
        return value ? `true` : `false`;
    }

    public static toBool(value: string): boolean {
        return value === `true`;
    }

    public static noAwait(callable: () => Promise<void>): void {
        const task = callable();
        task.catch(ex => console.log(JSON.stringify(ex)));
    }

    public static delay(duration: number): Promise<void> {
        return new Promise(resolve => setTimeout(resolve, duration * 1000));
    }

    public static sendRequestAsync(url: String, logger: ILogger): Promise<string> {
        return new Promise<string>((resolve, reject) => {
            let xhr = new XMLHttpRequest();
            xhr.onreadystatechange = () => {
                if (xhr.readyState == 4 && (xhr.status >= 200 && xhr.status < 400)) {
                    const response = xhr.responseText;
                    logger.debug(response);
                    resolve(response);
                } else {
                    logger.debug("error");
                    reject("error");
                }
            };
            xhr.onerror = (ev) => {
                logger.debug(`error: ${ev.type}`);
                reject(ev.type);
            };
            // @ts-ignore
            xhr.open("GET", url, true);
            xhr.send();
        });
    }
}