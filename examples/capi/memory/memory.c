#include <stdio.h>
#include <wasmedge/wasmedge.h>

// WasmEdge_Result parseJson(void *Data,
//                           const WasmEdge_CallingFrameContext *CallingFrameCxt,
//                           const WasmEdge_Value *In, WasmEdge_Value *Out) {
//   FILE *Fp = fopen("test.json", "r");
//   char Buffer[1024];
//   fread(Buffer, 1024, 1, Fp);
//   fclose(Fp);

//   void *Input = WasmEdge_ValueGetExternRef(In[0]);
//   char *Key = (char *)Input;
//   struct json_object *Value;
//   json_object *ParsedJson = json_tokener_parse(Buffer);
//   json_object_object_get_ex(ParsedJson, Key, &Value);
//   void *Output = (void *)json_object_get_string(Value);
//   Out[0] = WasmEdge_ValueGenExternRef(Output);
//   return WasmEdge_Result_Success;
// }

int main() {
  /* Create the VM context. */
  WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(NULL, NULL);

  // Host Module
  /* Create the import object. */
  WasmEdge_String ExportName = WasmEdge_StringCreateByCString("js");
  WasmEdge_ModuleInstanceContext *ImpObj =
      WasmEdge_ModuleInstanceCreate(ExportName);

  // memory
  WasmEdge_Limit MemLimit = {.HasMax = true, .Shared = false, .Min = 1, .Max = 2};
  WasmEdge_MemoryTypeContext *HostMemoryType = WasmEdge_MemoryTypeCreate(MemLimit);
  WasmEdge_MemoryInstanceContext *HostMemory =
      WasmEdge_MemoryInstanceCreate(HostMemoryType);
  WasmEdge_MemoryTypeDelete(HostMemoryType);

  WasmEdge_Result SetDataRes;
  const int COUNT = 5;
  uint8_t Buf[COUNT];

  for(int i = 0; i < COUNT; i++) {
    Buf[i] = 0x01;
    // 00000001 00000010 00000011
  }
  // for(int i = 0; i < 10; i++) {
  //   int offset = 4 * i;
  //   fprintf(stderr, "Buf[%d]=%d\n", i, Buf[i * offset + 4]);
  // }

  SetDataRes = WasmEdge_MemoryInstanceSetData(HostMemory, Buf, 0x0000, COUNT);
  if (WasmEdge_ResultOK(SetDataRes)) {
    printf("SetDataRes succeeded2\n");
  } else {
    printf("Error message: %s\n", WasmEdge_ResultGetMessage(SetDataRes));
  }

  /* Add the memory created above with the export name "mem". */
  WasmEdge_String HostMemoryName = WasmEdge_StringCreateByCString("mem");
  WasmEdge_ModuleInstanceAddMemory(ImpObj, HostMemoryName, HostMemory);
  WasmEdge_StringDelete(HostMemoryName);    

  WasmEdge_VMRegisterModuleFromImport(VMCxt, ImpObj);

  /* The parameters and returns arrays. */
  WasmEdge_Value Params[2] = {WasmEdge_ValueGenI32(0),
                              WasmEdge_ValueGenI32(4)};
  WasmEdge_Value Returns[1] = {WasmEdge_ValueGenI32(0)};
  WasmEdge_String FuncName = WasmEdge_StringCreateByCString("accumulate");
  WasmEdge_Result Res = WasmEdge_VMRunWasmFromFile(
      VMCxt, "memory.wasm", FuncName, Params, 2, Returns, 1);
  if (WasmEdge_ResultOK(Res)) {
    int32_t ResultValueI32 = WasmEdge_ValueGetI32(Returns[0]);
    printf("Got the result: %d\n",
          ResultValueI32);
  } else {
    printf("Error message: %s\n", WasmEdge_ResultGetMessage(Res));
  }

  /* Resources deallocations. */
  WasmEdge_VMDelete(VMCxt);
  WasmEdge_StringDelete(FuncName);
  WasmEdge_ModuleInstanceDelete(ImpObj);
  return 0;
}
