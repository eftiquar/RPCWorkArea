# RPCWorkArea

1. Skelatal code for basic RPC. The details of parameter marshalling and unmarshaling over the wire are omitted. That gap will be filled by gRPC machinery. We dont want to tie our implementation to gRPC. If we want to switch to another library, it should be painless. Only the marshalling\unmarshaling and communication part should change.  

2. An RPC interface is implemented on client side as a stub, which communicates over wire to actual implementation on server side 
3. Interfaces serve as contract and client is unaware of where and how the implementation runs. 

4. On client side, a thread safe mechanism to instantiate services is illustrated in "RPCServerStub::QueryService"

5. We provide an example of "Double checked locking" used for singleton instantiations

6. We also showcase usage of static instance for singleton pattern, thus leveraging C++'s inherent mechanisms of thread safety when static variables are initialized concurently

7. Using C++ lambda to customize cleanup the implementation of an interface. This is to hide the details of how an implementation is instantiated and cleaned up. It need not be on free store. 

8. We use C++'s synchronization primitive std::mutex. This makes our code platform neutral. Should there be a feature that is missing in standard C++, boost can be used to retain platfrom neutrality.  

9. Each service is identified via a GUID and its contract is exposed as an interface. 

10. PIMPL idiom can be used to further isolate the clients from inerface implementation. 

11. During my previous job, I had to reverse engineer protobuf, which is used by gRPC under the hood. It is a simple structure, a prefix sequence of bytes specifies what type of data follows, the prefix also includes length of the following data. Each data type has unique identifier code. A string is merely length prefixed reprsentation. Maps, lists, and arrays can be represented recursivey. 
