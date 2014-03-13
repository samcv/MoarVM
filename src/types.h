/* struct and union types are forward-declared for convenience */

typedef struct MVMActiveHandler MVMActiveHandler;
typedef struct MVMArgInfo MVMArgInfo;
typedef struct MVMArgProcContext MVMArgProcContext;
typedef struct MVMArray MVMArray;
typedef struct MVMArrayBody MVMArrayBody;
typedef struct MVMArrayREPRData MVMArrayREPRData;
typedef struct MVMAttributeIdentifier MVMAttributeIdentifier;
typedef struct MVMBoolificationSpec MVMBoolificationSpec;
typedef struct MVMBootTypes MVMBootTypes;
typedef struct MVMBytecodeAnnotation MVMBytecodeAnnotation;
typedef struct MVMCallCapture MVMCallCapture;
typedef struct MVMCallCaptureBody MVMCallCaptureBody;
typedef struct MVMCallsite MVMCallsite;
typedef struct MVMCFunction MVMCFunction;
typedef struct MVMCFunctionBody MVMCFunctionBody;
typedef struct MVMCode MVMCode;
typedef struct MVMCodeBody MVMCodeBody;
typedef struct MVMCollectable MVMCollectable;
typedef struct MVMCompUnit MVMCompUnit;
typedef struct MVMCompUnitBody MVMCompUnitBody;
typedef struct MVMConcatState MVMConcatState;
typedef struct MVMContainerConfigurer MVMContainerConfigurer;
typedef struct MVMContainerSpec MVMContainerSpec;
typedef struct MVMContainerRegistry MVMContainerRegistry;
typedef struct MVMContext MVMContext;
typedef struct MVMContextBody MVMContextBody;
typedef struct MVMContinuationTag MVMContinuationTag;
typedef struct MVMDLLRegistry MVMDLLRegistry;
typedef struct MVMDLLSym MVMDLLSym;
typedef struct MVMDLLSymBody MVMDLLSymBody;
typedef struct MVMException MVMException;
typedef struct MVMExceptionBody MVMExceptionBody;
typedef struct MVMExtOpRecord MVMExtOpRecord;
typedef struct MVMExtOpRegistry MVMExtOpRegistry;
typedef struct MVMExtRegistry MVMExtRegistry;
typedef struct MVMFrame MVMFrame;
typedef struct MVMFrameHandler MVMFrameHandler;
typedef struct MVMGen2Allocator MVMGen2Allocator;
typedef struct MVMGen2SizeClass MVMGen2SizeClass;
typedef struct MVMGCPassedWork MVMGCPassedWork;
typedef struct MVMGCWorklist MVMGCWorklist;
typedef struct MVMHash MVMHash;
typedef struct MVMHashAttrStore MVMHashAttrStore;
typedef struct MVMHashAttrStoreBody MVMHashAttrStoreBody;
typedef struct MVMHashBody MVMHashBody;
typedef struct MVMHashEntry MVMHashEntry;
typedef struct MVMHLLConfig MVMHLLConfig;
typedef struct MVMIntConstCache MVMIntConstCache;
typedef struct MVMInstance MVMInstance;
typedef struct MVMInvocationSpec MVMInvocationSpec;
typedef struct MVMIter MVMIter;
typedef struct MVMIterBody MVMIterBody;
typedef struct MVMKnowHOWAttributeREPR MVMKnowHOWAttributeREPR;
typedef struct MVMKnowHOWAttributeREPRBody MVMKnowHOWAttributeREPRBody;
typedef struct MVMKnowHOWREPR MVMKnowHOWREPR;
typedef struct MVMKnowHOWREPRBody MVMKnowHOWREPRBody;
typedef struct MVMLexicalRegistry MVMLexicalRegistry;
typedef struct MVMLexotic MVMLexotic;
typedef struct MVMLexoticBody MVMLexoticBody;
typedef struct MVMLoadedCompUnitName MVMLoadedCompUnitName;
typedef struct MVMNFA MVMNFA;
typedef struct MVMNFABody MVMNFABody;
typedef struct MVMNFAStateInfo MVMNFAStateInfo;
typedef struct MVMNativeCall MVMNativeCall;
typedef struct MVMNativeCallBody MVMNativeCallBody;
typedef struct MVMCStr MVMCStr;
typedef struct MVMCStrBody MVMCStrBody;
typedef struct MVMCPointer MVMCPointer;
typedef struct MVMCPointerBody MVMCPointerBody;
typedef struct MVMCArray MVMCArray;
typedef struct MVMCArrayBody MVMCArrayBody;
typedef struct MVMCArrayREPRData MVMCArrayREPRData;
typedef struct MVMCStruct MVMCStruct;
typedef struct MVMCStructBody MVMCStructBody;
typedef struct MVMCStructNameMap MVMCStructNameMap;
typedef struct MVMCStructREPRData MVMCStructREPRData;
typedef struct MVMMultiCache MVMMultiCache;
typedef struct MVMMultiCacheBody MVMMultiCacheBody;
typedef struct MVMMultiArityCache MVMMultiArityCache;
typedef struct MVMContinuation MVMContinuation;
typedef struct MVMContinuationBody MVMContinuationBody;
typedef struct MVMObject MVMObject;
typedef struct MVMObjectStooge MVMObjectStooge;
typedef struct MVMOpInfo MVMOpInfo;
typedef struct MVMOSHandle MVMOSHandle;
typedef struct MVMOSHandleBody MVMOSHandleBody;
typedef struct MVMP6bigint MVMP6bigint;
typedef struct MVMP6bigintBody MVMP6bigintBody;
typedef struct MVMP6int MVMP6int;
typedef struct MVMP6intBody MVMP6intBody;
typedef struct MVMP6intREPRData MVMP6intREPRData;
typedef struct MVMP6num MVMP6num;
typedef struct MVMP6numBody MVMP6numBody;
typedef struct MVMP6numREPRData MVMP6numREPRData;
typedef struct MVMP6opaque MVMP6opaque;
typedef struct MVMP6opaqueBody MVMP6opaqueBody;
typedef struct MVMP6opaqueBoxedTypeMap MVMP6opaqueBoxedTypeMap;
typedef struct MVMP6opaqueNameMap MVMP6opaqueNameMap;
typedef struct MVMP6opaqueREPRData MVMP6opaqueREPRData;
typedef struct MVMP6str MVMP6str;
typedef struct MVMP6strBody MVMP6strBody;
typedef union  MVMRegister MVMRegister;
typedef struct MVMReprRegistry MVMReprRegistry;
typedef struct MVMREPROps MVMREPROps;
typedef struct MVMREPROps_Associative MVMREPROps_Associative;
typedef struct MVMREPROps_Attribute MVMREPROps_Attribute;
typedef struct MVMREPROps_Boxing MVMREPROps_Boxing;
typedef struct MVMREPROps_Positional MVMREPROps_Positional;
typedef struct MVMSerializationContext MVMSerializationContext;
typedef struct MVMSerializationContextBody MVMSerializationContextBody;
typedef struct MVMSerializationReader MVMSerializationReader;
typedef struct MVMSerializationRoot MVMSerializationRoot;
typedef struct MVMSerializationWriter MVMSerializationWriter;
typedef struct MVMSTable MVMSTable;
typedef struct MVMStaticFrame MVMStaticFrame;
typedef struct MVMStaticFrameBody MVMStaticFrameBody;
typedef struct MVMStorageSpec MVMStorageSpec;
typedef struct MVMStrand MVMStrand;
typedef struct MVMString MVMString;
typedef struct MVMStringBody MVMStringBody;
typedef struct MVMStringConsts MVMStringConsts;
typedef struct MVMThread MVMThread;
typedef struct MVMThreadBody MVMThreadBody;
typedef struct MVMThreadContext MVMThreadContext;
typedef struct MVMUnicodeNamedValue MVMUnicodeNamedValue;
typedef struct MVMUnicodeNameRegistry MVMUnicodeNameRegistry;
typedef struct MVMUninstantiable MVMUninstantiable;
typedef struct MVMWorkThread MVMWorkThread;
typedef struct MVMIOOps MVMIOOps;
typedef struct MVMIOClosable MVMIOClosable;
typedef struct MVMIOEncodable MVMIOEncodable;
typedef struct MVMIOSyncReadable MVMIOSyncReadable;
typedef struct MVMIOSyncWritable MVMIOSyncWritable;
typedef struct MVMIOSeekable MVMIOSeekable;
typedef struct MVMIOSockety MVMIOSockety;
typedef struct MVMIOInteractive MVMIOInteractive;
typedef struct MVMIOLockable MVMIOLockable;
typedef struct MVMIOSyncStreamData MVMIOSyncStreamData;
typedef struct MVMIOSyncPipeData MVMIOSyncPipeData;
typedef struct MVMDecodeStream MVMDecodeStream;
typedef struct MVMDecodeStreamBytes MVMDecodeStreamBytes;
typedef struct MVMDecodeStreamChars MVMDecodeStreamChars;
