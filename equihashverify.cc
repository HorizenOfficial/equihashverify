#include <nan.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdint.h>
#include <sodium.h>
#include "crypto/equihash.h"

/*extern "C" {
    #include "src/equi/equi.h"
}*/

using namespace v8;

void verifyEH(const unsigned char *hdr, const unsigned char *soln){
  const int n = 200;
  const int k = 9;
  const int collisionBitLength = n / (k + 1);
  const int collisionByteLength = (collisionBitLength + 7) / 8;
  const int hashLength = (k + 1) * collisionByteLength;
  const int indicesPerHashOutput = 512 / n;
  const int hashOutput = indecesPerHashOutput * n / 8;
  const int equihashSolutionSize = (1 << k) * (n / (k + 1) + 1) / 8;
  const int solnr = 1 << k;
  uint32_t indices[512];

  crypto_generichash_blake2b_state state;
  Eh200_9.InitialiseState(state);
  crypto_generichash_blake2b_update(&state, hdr, 140);

  ExpandArray(soln, equihashSolutionSize, (unsigned char *)&indices, sizeof(indices), collisionBitLength + 1, 1);
  
  uint8_t vHash[hashLength];
  memset(vHash, 0 , sizeof(vHash));

   for (int j = 0; j < solnr; j++) {
  	uint8_t tmpHash[hashOutput];
  	uint8_t hash[hashLength];
  	int i = be32toh(indices[j]);
  	GenerateHash(&state, i / indicesPerHashOutput, tmpHash, hashOutput);
  	ExpandArray(tmpHash + (i % indicesPerHashOutput * n / 8), n / 8, hash, hashLength, collisionBitLength, 0);
  	for (int k = 0; k < hashLength; ++k)
  	    vHash[k] ^= hash[k];
  }
  return IsZero(sizeof(vHash));
}

void Verify(const v8::FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 2) {
  isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Wrong number of arguments")));
  return;
  }

  Local<Object> header = args[0]->ToObject();
  Local<Object> solution = args[1]->ToObject();

  if(!node::Buffer::HasInstance(header) || !node::Buffer::HasInstance(solution)) {
  isolate->ThrowException(Exception::TypeError(
    String::NewFromUtf8(isolate, "Arguments should be buffer objects.")));
  return;
  }

  const char *hdr = node::Buffer::Data(header);
  const char *soln = node::Buffer::Data(solution);

  bool result = verifyEH(hdr, soln);
  args.GetReturnValue().Set(result);

}


void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "verify", Verify);
}

NODE_MODULE(equihashverify, Init)
