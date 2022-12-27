<template>
    <BasePage :title="$t('ntpinfo.NtpInformation')" :isLoading="dataLoading">
        <CardElement :text="$t('ntpinfo.ConfigurationSummary')" textVariant="text-bg-primary">
            <div class="table-responsive">
                <table class="table table-hover table-condensed">
                    <tbody>
                        <tr>
                            <th>{{ $t('ntpinfo.Server') }}</th>
                            <td>{{ ntpDataList.ntp_server }}</td>
                        </tr>
                        <tr>
                            <th>{{ $t('ntpinfo.Timezone') }}</th>
                            <td>{{ ntpDataList.ntp_timezone }}</td>
                        </tr>
                        <tr>
                            <th>{{ $t('ntpinfo.TimezoneDescription') }}</th>
                            <td>{{ ntpDataList.ntp_timezone_descr }}</td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </CardElement>

        <CardElement :text="$t('ntpinfo.CurrentTime')" textVariant="text-bg-primary" add-space>
            <div class="table-responsive">
                <table class="table table-hover table-condensed">
                    <tbody>
                        <tr>
                            <th>{{ $t('ntpinfo.Status') }}</th>
                            <td class="badge" :class="{
                                'text-bg-danger': !ntpDataList.ntp_status,
                                'text-bg-success': ntpDataList.ntp_status,
                            }">
                                <span v-if="ntpDataList.ntp_status">{{ $t('ntpinfo.Synced') }}</span>
                                <span v-else>{{ $t('ntpinfo.NotSynced') }}</span>
                            </td>
                        </tr>
                        <tr>
                            <th>{{ $t('ntpinfo.LocalTime') }}</th>
                            <td>{{ ntpDataList.ntp_localtime }}</td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </CardElement>

        <CardElement :text="$t('ntpinfo.SunriseInformation')" textVariant="text-bg-primary" add-space>
            <div class="table-responsive">
                <table class="table table-hover table-condensed">
                    <tbody>
                        <tr>
                            <th>Status</th>
                            <td class="badge" :class="{
                                'text-bg-danger': !ntpDataList.sunset_enabled,
                                'text-bg-success': ntpDataList.sunset_enabled,
                            }">
                                <span v-if="ntpDataList.sunset_enabled">{{ $t('ntpinfo.Enabled') }}</span>
                                <span v-else>{{ $t('ntpinfo.Disabled') }}</span>
                            </td>
                        </tr>
                        <tr>
                            <th>{{ $t('ntpinfo.TimezoneOffset') }}</th>
                            <td>{{ ntpDataList.timezone_offset }}</td>
                        </tr>
                        <tr>
                            <th>{{ $t('ntpinfo.SunriseTime') }}</th>
                            <td>{{ $t('ntpinfo.Minutes', { tim: ntpDataList.sunrise_time }) }}</td>
                        </tr>
                        <tr>
                            <th>{{ $t('ntpinfo.SunsetTime') }}</th>
                            <td>{{ $t('ntpinfo.Minutes', { tim: ntpDataList.sunset_time }) }}</td>
                        </tr>
                        <tr>
                            <th>Status</th>
                            <td class="badge" :class="{
                                'text-bg-warning': !ntpDataList.sunset_isdaytime,
                                'text-bg-success': ntpDataList.sunset_isdaytime,
                            }">
                                <span v-if="ntpDataList.sunset_isdaytime">{{ $t('ntpinfo.Daytime') }}</span>
                                <span v-else>{{ $t('ntpinfo.Nighttime') }}</span>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </CardElement>
    </BasePage>
</template>

<script lang="ts">
import BasePage from '@/components/BasePage.vue';
import CardElement from '@/components/CardElement.vue';
import type { NtpStatus } from "@/types/NtpStatus";
import { authHeader, handleResponse } from '@/utils/authentication';
import { defineComponent } from 'vue';

export default defineComponent({
    components: {
        BasePage,
        CardElement,
    },
    data() {
        return {
            dataLoading: true,
            ntpDataList: {} as NtpStatus,
        };
    },
    created() {
        this.getNtpInfo();
    },
    methods: {
        getNtpInfo() {
            this.dataLoading = true;
            fetch("/api/ntp/status", { headers: authHeader() })
                .then((response) => handleResponse(response, this.$emitter, this.$router))
                .then((data) => {
                    this.ntpDataList = data;
                    this.dataLoading = false;
                });
        },
    },
});
</script>