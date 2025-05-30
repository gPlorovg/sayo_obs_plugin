#!/bin/bash

BUILD_ARGS="--skip-deps"
if [[ "$1" == "--with-deps" ]]; then
    BUILD_ARGS=""
fi
/home/plorov/Study/ASR_OBS_Plugin/.github/scripts/build-ubuntu $BUILD_ARGS

cp -f /home/plorov/Study/ASR_OBS_Plugin/release/RelWithDebInfo/lib/x86_64-linux-gnu/obs-plugins/asr_obs_plugin.so /usr/lib/x86_64-linux-gnu/obs-plugins/.
cp -rf /home/plorov/Study/ASR_OBS_Plugin/release/RelWithDebInfo/share/obs/obs-plugins/asr_obs_plugin/locale /usr/share/obs/obs-plugins/asr_obs_plugin/.
echo "Plugin 'asr_obs_plugin' installed"
