type CallCppCallback = (tag: string, message: string) => void;

class MessageBridgeImplNative implements IMessageBridgeImpl {
    public constructor(callback: CallCppCallback) {
        // FIXME.
    }

    public call(tag: string, message: string): string {
        throw new Error("Method not implemented.");
    }
}