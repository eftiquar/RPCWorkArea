# RPCWorkArea

1. Skelatal code for basic RPC. The details of parameter marshalling and unmarshaling over the wire are omitted.
2. An RPC interface is implemented on client side as a stub, which communicates over wire to actual implementation on server side.
Interfaces serve as contract and client is unaware of where and how the implementation runs
On client side, a thread safe mechanism to instantiate services is illustrated in "RPCServerStub::QueryService"
We provide an example of "Double checked locking" used for singleton instantiations
We also showcase usage of static instance for singleton pattern, thus leveraging C++'s inherent mechanisms of thread safety when static variables are initialized concurently
Using C++ lambda to customize cleanup the implementation of an interface. This is to hide the details of how an implementation is initialized.
We use C++'s synchronization primitive std::mutex
A each service is identified via a GUID and its contract is exposed as an interface. 
