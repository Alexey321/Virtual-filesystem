# Virtual-filesystem
Simple implementation of virtual filesystem

  A Virtual File System (VFS) or virtual filesystem switch is an abstraction layer on top of a more concrete file system. The purpose of a VFS is to allow client applications to access different types of concrete file systems in a uniform way. A VFS can, for example, be used to access local and network storage devices transparently without the client application noticing the difference. It can be used to bridge the differences in Windows, classic Mac OS/macOS and Unix filesystems, so that applications can access files on local file systems of those types without having to know what type of file system they are accessing.

  A VFS specifies an interface (or a "contract") between the kernel and a concrete file system. Therefore, it is easy to add support for new file system types to the kernel simply by fulfilling the contract. The terms of the contract might change incompatibly from release to release, which would require that concrete file system support be recompiled, and possibly modified before recompilation, to allow it to work with a new release of the operating system; or the supplier of the operating system might make only backward-compatible changes to the contract, so that concrete file system support built for a given release of the operating system would work with future versions of the operating system. 
  
### Single-file virtual file systems

Sometimes Virtual File System refers to a file or a group of files (not necessarily inside a concrete file system) that acts as a manageable container which should provide the functionality of a concrete file system through the usage of software. Examples of such containers are SolFS or a single-file virtual file system in an emulator like PCTask or so-called WinUAE, Oracle's VirtualBox, Microsoft's Virtual PC, VMware.

The primary benefit for this type of file system is that it is centralized and easy to remove. A single-file virtual file system may include all the basic features expected of any file system (virtual or otherwise), but access to the internal structure of these file systems is often limited to programs specifically written to make use of the single-file virtual file system (instead of implementation through a driver allowing universal access). Another major drawback is that performance is relatively low when compared to other virtual file systems. Low performance is mostly due to the cost of shuffling virtual files when data is written or deleted from the virtual file system. 

### Implementation of single-file virtual filesystems

Direct examples of single-file virtual file systems include emulators, such as PCTask and WinUAE, which encapsulate not only the filesystem data but also emulated disk layout. This makes it easy to treat an OS installation like any other piece of software—transferring it with removable media or over the network. 

## Usage example



















