export class Utils {
    public static toString(value: boolean): string {
        return value ? `true` : `false`;
    }

    public static toBool(value: string): boolean {
        return value === `true`;
    }
}