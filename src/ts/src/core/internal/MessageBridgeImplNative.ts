type CallCppCallback = (tag: string, message: string) => void;

declare function ee_staticCall(tag: string, message: string): string;

export class MessageBridgeImplNative implements IMessageBridgeImpl {
    public constructor(callback: CallCppCallback) {
        // FIXME.
    }

    public call(tag: string, message: string): string {
        return ee_staticCall(tag, message);
    }
}