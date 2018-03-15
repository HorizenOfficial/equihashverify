#include <nan.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdint.h>
#include <sodium.h>
#include "crypto/equihash.h"

#include "arith_uint256.h"
#include "uint256.h"
#include <vector>
#include <boost/test/unit_test.hpp>

/*extern "C" {
    #include "src/equi/equi.h"
}*/

using namespace v8;

bool TestEquihashValidator(unsigned int n, unsigned int k, const std::string &I, const arith_uint256 &nonce, std::vector<uint32_t> soln, bool expected) {
    size_t cBitLen { n/(k+1) };
    crypto_generichash_blake2b_state state;
    EhInitialiseState(n, k, state);
    uint256 V = ArithToUint256(nonce);
    crypto_generichash_blake2b_update(&state, (unsigned char*)&I[0], I.size());
    crypto_generichash_blake2b_update(&state, V.begin(), V.size());
    BOOST_TEST_MESSAGE("Running validator: n = " << n << ", k = " << k << ", I = " << I << ", V = " << V.GetHex() << ", expected = " << expected << ", soln =");
    std::stringstream strm;
    //PrintSolution(strm, soln);
    BOOST_TEST_MESSAGE(strm.str());
    bool isValid;
    EhIsValidSolution(n, k, state, GetMinimalFromIndices(soln, cBitLen), isValid);
    BOOST_CHECK(isValid == expected);

    return isValid;
}

bool verifyEH(const char *hdr, const char *soln){
  unsigned int n = 200;
  unsigned int k = 9;

  std::vector<char>::size_type size = strlen((const char*) soln);
  std::vector<uint32_t> solution(soln, soln + size);

  return TestEquihashValidator(n, k, "Equihash is an asymmetric PoW based on the Generalised Birthday problem.", 1, solution, true);
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
