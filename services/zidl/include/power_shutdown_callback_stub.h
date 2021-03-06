/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef POWERMGR_SERVICES_IPOWER_SHUTDOWN_CALLBACK_STUB_H
#define POWERMGR_SERVICES_IPOWER_SHUTDOWN_CALLBACK_STUB_H

#include <iremote_stub.h>
#include <nocopyable.h>

#include "ishutdown_callback.h"

namespace OHOS {
namespace PowerMgr {
class PowerShutdownCallbackStub : public IRemoteStub<IShutdownCallback> {
public:
    DISALLOW_COPY_AND_MOVE(PowerShutdownCallbackStub);
    PowerShutdownCallbackStub() = default;
    virtual ~PowerShutdownCallbackStub() = default;
    int OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;
    void ShutdownCallback() override {};

private:
    int32_t OnPowerShutdownCallbackStub();
};
} // namespace PowerMgr
} // namespace OHOS
#endif // POWERMGR_SERVICES_IPOWER_STATE_CALLBACK_STUB_H
