#include "RansacMatcher.hpp"

#include <improc/algorithm/transformation/MatrixData.hpp>

namespace model {
namespace improc {
namespace matcher {

RansacMatcher::RansacMatcher(std::unique_ptr<algorithm::Ransac>&& ransac)
    : ransac(std::move(ransac))
{
}

MatchingPointsPairs RansacMatcher::matchPoints(const MatchingPointsPairs& pointsPairs)
{
    auto consensusModel = ransac->runRansac(pointsPairs);
    return consensusModel.second.getDataAsSamplesVec();
}

uint32_t RansacMatcher::getItersNum() const
{
    return ransac->getItersNum();
}

} // namespace matcher
} // namespace improc
} // namespace model
