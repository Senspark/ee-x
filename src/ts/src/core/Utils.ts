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
}