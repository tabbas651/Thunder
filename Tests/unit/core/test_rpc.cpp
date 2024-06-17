/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Metrological
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../IPTestAdministrator.h"

#include <gtest/gtest.h>

#include <core/core.h>
#include <com/com.h>
#include <core/Portability.h>

namespace Thunder {
namespace Tests {
namespace Core {

namespace Exchange {
    struct IAdder : virtual public Thunder::Core::IUnknown {
        enum { ID = 0x80000001 };
        virtual uint32_t GetValue() = 0;
        virtual void Add(uint32_t value) = 0;
        virtual uint32_t GetPid() = 0;
    };
} // Exchange

    class Adder : public Exchange::IAdder
    {
    public:
        Adder()
            : m_value(0)
        {
        }

        uint32_t GetValue()
        {
            return m_value;
        }

        void Add(uint32_t value)
        {
            m_value += value;
        }

        uint32_t GetPid()
        {
            return getpid();
        }

        BEGIN_INTERFACE_MAP(Adder)
            INTERFACE_ENTRY(Exchange::IAdder)
        END_INTERFACE_MAP

    private:
        uint32_t m_value;
    };

    // Proxystubs.
    using namespace Exchange;

    // -----------------------------------------------------------------
    // STUB
    // -----------------------------------------------------------------

    //
    // IAdder interface stub definitions
    //
    // Methods:
    //  (0) virtual uint32_t GetValue() = 0
    //  (1) virtual void Add(uint32_t) = 0
    //  (2) virtual uint32_t GetPid() = 0
    //

    ProxyStub::MethodHandler AdderStubMethods[] = {
        // virtual uint32_t GetValue() = 0
        //
        [](Thunder::Core::ProxyType<Thunder::Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Thunder::Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& input(message->Parameters());

            // call implementation
            IAdder* implementation = reinterpret_cast<IAdder*>(input.Implementation());
            EXPECT_TRUE((implementation != nullptr) && "Null IAdder implementation pointer");
            const uint32_t output = implementation->GetValue();

            // write return value
            RPC::Data::Frame::Writer writer(message->Response().Writer());
            writer.Number<const uint32_t>(output);
        },

        // virtual void Add(uint32_t) = 0
        //
        [](Thunder::Core::ProxyType<Thunder::Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Thunder::Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& input(message->Parameters());

            // read parameters
            RPC::Data::Frame::Reader reader(input.Reader());
            const uint32_t param0 = reader.Number<uint32_t>();

            // call implementation
            IAdder* implementation = reinterpret_cast<IAdder*>(input.Implementation());
            EXPECT_TRUE((implementation != nullptr) && "Null IAdder implementation pointer");
            implementation->Add(param0);
        },

        // virtual uint32_t GetPid() = 0
        //
        [](Thunder::Core::ProxyType<Thunder::Core::IPCChannel>& channel VARIABLE_IS_NOT_USED, Thunder::Core::ProxyType<RPC::InvokeMessage>& message) {
            RPC::Data::Input& input(message->Parameters());

            // call implementation
            IAdder* implementation = reinterpret_cast<IAdder*>(input.Implementation());
            EXPECT_TRUE((implementation != nullptr) && "Null IAdder implementation pointer");
            const uint32_t output = implementation->GetPid();

            // write return value
            RPC::Data::Frame::Writer writer(message->Response().Writer());
            writer.Number<const uint32_t>(output);
        },

        nullptr
    }; // AdderStubMethods[]

    // -----------------------------------------------------------------
    // PROXY
    // -----------------------------------------------------------------

    //
    // IAdder interface proxy definitions
    //
    // Methods:
    //  (0) virtual uint32_t GetValue() = 0
    //  (1) virtual void Add(uint32_t) = 0
    //  (2) virtual uint32_t GetPid() = 0
    //

    class AdderProxy final : public ProxyStub::UnknownProxyType<IAdder> {
    public:
        AdderProxy(const Thunder::Core::ProxyType<Thunder::Core::IPCChannel>& channel, Thunder::Core::instance_id implementation, const bool otherSideInformed)
            : BaseClass(channel, implementation, otherSideInformed)
        {
        }

        uint32_t GetValue() override
        {
            IPCMessage newMessage(BaseClass::Message(0));

            // invoke the method handler
            uint32_t output{};
            if ((output = Invoke(newMessage)) == Thunder::Core::ERROR_NONE) {
                // read return value
                RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
                output = reader.Number<uint32_t>();
            }

            return output;
        }

        void Add(uint32_t param0) override
        {
            IPCMessage newMessage(BaseClass::Message(1));

            // write parameters
            RPC::Data::Frame::Writer writer(newMessage->Parameters().Writer());
            writer.Number<const uint32_t>(param0);

            // invoke the method handler
            Invoke(newMessage);
        }

        uint32_t GetPid() override
        {
            IPCMessage newMessage(BaseClass::Message(2));

            // invoke the method handler
            uint32_t output{};
            if ((output = Invoke(newMessage)) == Thunder::Core::ERROR_NONE) {
                // read return value
                RPC::Data::Frame::Reader reader(newMessage->Response().Reader());
                output = reader.Number<uint32_t>();
            }

            return output;
        }
    }; // class AdderProxy

    // -----------------------------------------------------------------
    // REGISTRATION
    // -----------------------------------------------------------------

    namespace {

        typedef ProxyStub::UnknownStubType<IAdder, AdderStubMethods> AdderStub;

        static class Instantiation {
        public:
            Instantiation()
            {
                RPC::Administrator::Instance().Announce<IAdder, AdderProxy, AdderStub>();
            }
        } ProxyStubRegistration;

    } // namespace

    namespace {
        class ExternalAccess : public RPC::Communicator
        {
        public:
            ExternalAccess() = delete;
            ExternalAccess(const ExternalAccess &) = delete;
            ExternalAccess & operator=(const ExternalAccess &) = delete;

            ExternalAccess(const Thunder::Core::NodeId & source)
                : RPC::Communicator(source, _T(""))
            {
                Open(Thunder::Core::infinite);
            }

            ~ExternalAccess()
            {
                Close(Thunder::Core::infinite);
            }

        private:
            virtual void* Acquire(const string& className, const uint32_t interfaceId, const uint32_t versionId)
            {
                void* result = nullptr;

                if (interfaceId == Exchange::IAdder::ID) {
                    Exchange::IAdder * newAdder = Thunder::Core::Service<Adder>::Create<Exchange::IAdder>();
                    result = newAdder;
                }

                return result;
            }
        };
    }

    TEST(Core_RPC, adder)
    {
       std::string connector{"/tmp/wperpc01"};
       auto lambdaFunc = [connector](IPTestAdministrator & testAdmin) {
          Thunder::Core::NodeId remoteNode(connector.c_str());

          ExternalAccess communicator(remoteNode);

          testAdmin.Sync("setup server");

          testAdmin.Sync("done testing");

          communicator.Close(Thunder::Core::infinite);
       };

       static std::function<void (IPTestAdministrator&)> lambdaVar = lambdaFunc;

       IPTestAdministrator::OtherSideMain otherSide = [](IPTestAdministrator& testAdmin ) { lambdaVar(testAdmin); };

       IPTestAdministrator testAdmin(otherSide);

       testAdmin.Sync("setup server");

       {
          Thunder::Core::NodeId remoteNode(connector.c_str());

          Thunder::Core::ProxyType<Thunder::RPC::InvokeServerType<4, 0, 1>> engine = Thunder::Core::ProxyType<Thunder::RPC::InvokeServerType<4, 0, 1>>::Create();
          EXPECT_TRUE(engine.IsValid());
          Thunder::Core::ProxyType<Thunder::RPC::CommunicatorClient> client = Thunder::Core::ProxyType<Thunder::RPC::CommunicatorClient>::Create(remoteNode, Thunder::Core::ProxyType<Thunder::Core::IIPCServer>(engine));
          EXPECT_TRUE(client.IsValid());

          // Create remote instance of "IAdder".
          Exchange::IAdder * adder = client->Open<Exchange::IAdder>(_T("Adder"));

          ASSERT_TRUE(adder != nullptr);

          // Perform some arithmatic.
          EXPECT_EQ(adder->GetValue(), static_cast<uint32_t>(0));
          adder->Add(20);
          EXPECT_EQ(adder->GetValue(), static_cast<uint32_t>(20));
          adder->Add(22);
          EXPECT_EQ(adder->GetValue(), static_cast<uint32_t>(42));

          // Make sure other side is indeed running in other process.
          EXPECT_NE(adder->GetPid(), (uint32_t)getpid());

          adder->Release();

          client->Close(Thunder::Core::infinite);
       }

       testAdmin.Sync("done testing");
       Thunder::Core::Singleton::Dispose();
    }

} // Core
} // Tests
} // Thunder
