// Copyright (C) 2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <tuple>
#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "single_layer_tests/shuffle_channels.hpp"
#include "ngraph_functions/builders.hpp"

namespace LayerTestsDefinitions {

std::string ShuffleChannelsLayerTest::getTestCaseName(testing::TestParamInfo<shuffleChannelsLayerTestParamsSet> obj) {
    shuffleChannelsSpecificParams shuffleChannelsParams;
    InferenceEngine::Precision netPrecision;
    InferenceEngine::SizeVector inputShapes;
    std::string targetDevice;
    std::tie(shuffleChannelsParams, netPrecision, inputShapes, targetDevice) = obj.param;
    int axis, group;
    std::tie(axis, group) = shuffleChannelsParams;

    std::ostringstream result;
    result << "IS=" << CommonTestUtils::vec2str(inputShapes) << "_";
    result << "Axis=" << std::to_string(axis) << "_";
    result << "Group=" << std::to_string(group) << "_";
    result << "netPRC=" << netPrecision.name() << "_";
    result << "targetDevice=" << targetDevice;
    return result.str();
}

void ShuffleChannelsLayerTest::SetUp() {
    shuffleChannelsSpecificParams shuffleChannelsParams;
    std::vector<size_t> inputShape;
    auto netPrecision   = InferenceEngine::Precision::UNSPECIFIED;
    std::tie(shuffleChannelsParams, netPrecision, inputShape, targetDevice) = this->GetParam();
    int axis, group;
    std::tie(axis, group) = shuffleChannelsParams;
    auto ngPrc = FuncTestUtils::PrecisionUtils::convertIE2nGraphPrc(netPrecision);
    auto params = ngraph::builder::makeParams(ngPrc, {inputShape});
    auto paramOuts = ngraph::helpers::convert2OutputVector(
            ngraph::helpers::castOps2Nodes<ngraph::op::Parameter>(params));
    auto shuffleChannels = std::dynamic_pointer_cast<ngraph::opset3::ShuffleChannels>(
            ngraph::builder::makeShuffleChannels(paramOuts[0], axis, group));
    ngraph::ResultVector results{std::make_shared<ngraph::opset3::Result>(shuffleChannels)};
    function = std::make_shared<ngraph::Function>(results, params, "shuffleChannels");
}

TEST_P(ShuffleChannelsLayerTest, CompareWithRefs) {
    Run();
}
}  // namespace LayerTestsDefinitions
