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

#include "suspend/running_lock_hub.h"

#include <file_ex.h>
#include <sys/capability.h>
#include <sys/eventfd.h>

#include "errors.h"
#include "pubdef.h"

#include "hilog_wrapper.h"

namespace OHOS {
namespace PowerMgr {
namespace Suspend {
void RunningLockHub::Acquire(const std::string& name)
{
    {
        std::lock_guard lock(mutex_);
        if (!SaveLockFile(name, true)) {
            POWER_HILOGE(MODULE_SERVICE, "Failed to write the lock state!");
        }
        runningLockMap_[name]++;
    }
    NotifySuspendCounter(true);
    POWER_HILOGI(MODULE_SERVICE, "Acquire runningLock, id: %{public}s", name.c_str());
}

void RunningLockHub::Release(const std::string& name)
{
    {
        std::lock_guard lock(mutex_);
        if (InitFd()) {
            if (!SaveStringToFd(unlockFd_, name.c_str())) {
                POWER_HILOGE(MODULE_SERVICE, "Failed to write the unlock state!");
            }
        }
        if (runningLockMap_.find(name) == runningLockMap_.end()) {
            return;
        }
        runningLockMap_[name]--;
        if (runningLockMap_[name] == 0) {
            runningLockMap_.erase(name);
        }
    }
    NotifySuspendCounter(false);
    POWER_HILOGI(MODULE_SERVICE, "Release runningLock, id: %{public}s", name.c_str());
}

bool RunningLockHub::InitFd()
{
    static bool inited = false;
    if (inited) {
        return (lockFd_ >= 0 && lockFd_ >= 0);
    }
    lockFd_ = UniqueFd(TEMP_FAILURE_RETRY(open(LOCK_PATH, O_RDWR | O_CLOEXEC)));
    unlockFd_ = UniqueFd(TEMP_FAILURE_RETRY(open(UNLOCK_PATH, O_RDWR | O_CLOEXEC)));
    inited = true;
    if (lockFd_ < 0 || unlockFd_ < 0) {
        POWER_HILOGE(MODULE_SERVICE, "Running lock fd initialization failed!");
        return false;
    }
    return inited;
}

bool RunningLockHub::SaveLockFile(const std::string& name, bool isAcquire)
{
    if (InitFd()) {
        int32_t fd = isAcquire ? lockFd_ : unlockFd_;
        return SaveStringToFd(fd, name.c_str());
    }
    return false;
}

void RunningLockHub::NotifySuspendCounter(bool isAcquire)
{
    isAcquire ? sc_->IncSuspendBlockCounter() : sc_->DecSuspendBlockCounter();
}
} // namespace Suspend
} // namespace PowerMgr
} // namespace OHOS