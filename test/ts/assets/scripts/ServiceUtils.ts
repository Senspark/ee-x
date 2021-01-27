import * as ee from '@senspark/ee-x';

export class ServiceUtils {
    public static initialize(): void {
        ee.PluginManager.initializePlugins();
    }
}