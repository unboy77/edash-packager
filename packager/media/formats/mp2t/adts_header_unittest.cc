// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>

#include "packager/base/compiler_specific.h"
#include "packager/base/logging.h"
#include "packager/base/strings/string_number_conversions.h"
#include "packager/media/formats/mp2t/adts_header.h"

namespace {

const char kValidAdtsFrame[] =
    "fff15080429ffcda004c61766335332e33352e30004258892a5361062403"
    "d040000000001ff9055e9fe77ac56eb1677484e0ef0c3102a39daa8355a5"
    "37ecab2b156e4ba73ceedb24ea51194e57c9385fa67eca8edc914902e852"
    "3185b52299516e679fb3768aa9f13ccac5b257410080282c9a50318ec94e"
    "ba24ea305bafab2b2beab16557ef9ecaa8f17bedea84788c8d42e4b3c65b"
    "1e7ecae7528b909bc46c76cca73b906ec980ed9f32b25ecd28f43f9516de"
    "3ff249f23bb9c93e64c4808195f284653c40592c1a8dc847f5f11791fd80"
    "b18e02c1e1ed9f82c62a1f8ea0f5b6dbf2112c2202973b00de71bb49f906"
    "ed1bc63768dda378c8f9c6ed1bb48f68dda378c9f68dda3768dda3768de3"
    "23da3768de31bb492a5361062403d040000000001ff9055e9fe77ac56eb1"
    "677484e0ef0c3102a39daa8355a537ecab2b156e4ba73ceedb24ea51194e"
    "57c9385fa67eca8edc914902e8523185b52299516e679fb3768aa9f13cca"
    "c5b257410080282c9a50318ec94eba24ea305bafab2b2beab16557ef9eca"
    "a8f17bedea84788c8d42e4b3c65b1e7ecae7528b909bc46c76cca73b906e"
    "c980ed9f32b25ecd28f43f9516de3ff249f23bb9c93e64c4808195f28465"
    "3c40592c1a8dc847f5f11791fd80b18e02c1e1ed9f82c62a1f8ea0f5b6db"
    "f2112c2202973b00de71bb49f906ed1bc63768dda378c8f9c6ed1bb48f68"
    "dda378c9f68dda3768dda3768de323da3768de31bb4e";

const uint8_t kExpectedAudioSpecificConfig[] = {0x12, 0x10};

}  // anonymous namespace

namespace edash_packager {
namespace media {
namespace mp2t {

class AdtsHeaderTest : public testing::Test {
 public:
  virtual void SetUp() OVERRIDE {
    ASSERT_TRUE(base::HexStringToBytes(kValidAdtsFrame, &adts_frame_));
  }

 protected:
  std::vector<uint8_t> adts_frame_;
};

TEST_F(AdtsHeaderTest, ParseSuccess) {
  const uint8_t kExpectedObjectType(2);
  const uint32_t kExpectedSamplingFrequency(44100);
  const uint8_t kExpectedNumChannels(2);
  AdtsHeader adts_header;
  EXPECT_TRUE(adts_header.Parse(adts_frame_.data(), adts_frame_.size()));
  EXPECT_EQ(kExpectedObjectType, adts_header.GetObjectType());
  EXPECT_EQ(kExpectedSamplingFrequency, adts_header.GetSamplingFrequency());
  EXPECT_EQ(kExpectedNumChannels, adts_header.GetNumChannels());
  std::vector<uint8_t> audio_specific_config;
  ASSERT_TRUE(adts_header.GetAudioSpecificConfig(&audio_specific_config));
  EXPECT_EQ(arraysize(kExpectedAudioSpecificConfig),
            audio_specific_config.size());
  EXPECT_EQ(std::vector<uint8_t>(kExpectedAudioSpecificConfig,
                                 kExpectedAudioSpecificConfig +
                                     arraysize(kExpectedAudioSpecificConfig)),
            audio_specific_config);
}

TEST_F(AdtsHeaderTest, ParseFailFrameSize) {

  AdtsHeader adts_header;
  EXPECT_FALSE(adts_header.Parse(adts_frame_.data(), adts_frame_.size() - 1));
  EXPECT_FALSE(adts_header.Parse(adts_frame_.data(), adts_frame_.size() + 1));
  EXPECT_FALSE(adts_header.Parse(adts_frame_.data(), 1));
}

TEST_F(AdtsHeaderTest, GetFrameSizeSuccess) {
   EXPECT_EQ(adts_frame_.size(),
             AdtsHeader::GetAdtsFrameSize(adts_frame_.data(),
                                          adts_frame_.size()));
}

TEST_F(AdtsHeaderTest, GetHeaderSizeSuccess) {
   const size_t kExpectedHeaderSize(7);
   EXPECT_EQ(kExpectedHeaderSize,
             AdtsHeader::GetAdtsHeaderSize(adts_frame_.data(),
                                           adts_frame_.size()));
}

}  // Namespace mp2t
}  // namespace media
}  // namespace edash_packager
