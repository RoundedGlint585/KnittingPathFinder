//
// Created by roundedglint585 on 6/11/19.
//

#include "../include/PathFinder.hpp"

std::optional<std::vector<std::pair<std::size_t, std::size_t>>>
PathFinder::operator()(const std::vector<std::vector<uint8_t >> &bitmap) {
    if (bitmap.empty()) {
        return std::nullopt;
    }
    height_ = bitmap.size();
    width_ = bitmap[0].size();

    for (size_t i = 0; i < bitmap.size(); i++) {
        for (size_t j = 0; j < bitmap[i].size(); j++) {
            if (bitmap[i][j]) {
                path_ = {{i, j}};
                break;
            }
        }
        if (!path_.empty()) {
            break;
        }
    }

    if (path_.empty()) {
        return std::nullopt;
    }
    visited_ = bitmap;
    for (auto &i: visited_) {
        for (auto &j: i) {
            j = 0;
        }
    }
    std::stack<std::pair<size_t, size_t>> depthPath;
    while (visited_[path_[0].first][path_[0].second] != 3) {
        auto position = path_.back();

        ///Trying to find new cell, that not visited
        if (checkCloseLeftDown(position, bitmap, 0)) {
            depthPath.emplace(position.first, position.second - 1);
            continue;
        }
        if (checkFarLeftDown(position, bitmap, 0)) {
            depthPath.emplace(position.first, position.second - 2);
            continue;
        }
        if (checkCloseLeftUp(position, bitmap, 0)) {
            depthPath.emplace(position.first - 1, position.second - 1);
            continue;
        }
        if (checkFarLeftUp(position, bitmap, 0)) {
            depthPath.emplace(position.first - 1, position.second - 2);
            continue;
        }
        if (checkCloseRightDown(position, bitmap, 0)) {
            depthPath.emplace(position.first, position.second);
            continue;
        }
        if (checkFarRightDown(position, bitmap, 0)) {
            depthPath.emplace(position.first, position.second + 1);
            continue;
        }

        if (checkCloseRightUp(position, bitmap, 0)) {
            depthPath.emplace(position.first - 1, position.second);
            continue;
        }

        if (checkFarRightUp(position, bitmap, 0)) {
            depthPath.emplace(position.first - 1, position.second + 1);
            continue;
        }
        ///
        ///If there are no such cells, then
        if (!depthPath.empty()) {
            auto diff = std::make_pair<int, int>(position.first - depthPath.top().first,
                                                 position.second - depthPath.top().second);
            depthPath.pop();
            if (diff.second == 0 && diff.first == 0) {
                checkCloseRightDown(position, bitmap, 2);
                continue;
            }
            if (diff.first == 1 && diff.second == 0) {
                checkCloseRightUp(position, bitmap, 1);
                continue;
            }
            if (diff.first == 0 && diff.second == 1) {
                checkCloseLeftDown(position, bitmap, 1);
                continue;
            }
            if (diff.first == 1 && diff.second == 1) {
                checkCloseLeftUp(position, bitmap, 2);
                continue;
            }
            if (diff.first == 0 && diff.second == -1) {
                checkFarRightDown(position, bitmap, 1);
                continue;
            }
            if (diff.first == 1 && diff.second == -1) {
                checkFarRightUp(position, bitmap, 2);
                continue;
            }
            if (diff.first == 0 && diff.second == 2) {
                checkFarLeftDown(position, bitmap, 2);
                continue;
            }
            if (diff.first == 1 && diff.second == 2) {
                checkFarLeftUp(position, bitmap, 1);
                continue;
            }
        }
        return std::nullopt;
    }
    if (!isFullyCovered(bitmap)) {
        return std::nullopt;
    }
    return path_;


}

bool PathFinder::checkCloseLeftUp(const std::pair<std::size_t, std::size_t> &position,
                                  const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.second > 0) && (position.first > 0)) {
        if (bitmap[position.first - 1][position.second - 1] &&
            (visited_[position.first - 1][position.second - 1] == visitationStatus)) {
            visited_[position.first - 1][position.second - 1] = (visitationStatus == 0) ? 1 : 3; //
            path_.emplace_back(position.first, position.second - 1);
            path_.emplace_back(position.first - 1, position.second);
            return true;
        }
    }
    return false;
}

bool PathFinder::checkCloseRightUp(const std::pair<std::size_t, std::size_t> &position,
                                   const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.first > 0) && (position.second < width_)) {
        if (bitmap[position.first - 1][position.second] &&
            (visited_[position.first - 1][position.second] == visitationStatus)) {
            visited_[position.first - 1][position.second] = (visitationStatus == 0) ? 2 : 3; //
            path_.emplace_back(position.first, position.second + 1);
            path_.emplace_back(position.first - 1, position.second);
            return true;
        }
    }
    return false;
}

bool PathFinder::checkCloseLeftDown(const std::pair<std::size_t, std::size_t> &position,
                                    const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.first < height_) && (position.second > 0)) {
        if (bitmap[position.first][position.second - 1] &&
            (visited_[position.first][position.second - 1] == visitationStatus)) {
            visited_[position.first][position.second - 1] = (visitationStatus == 0) ? 2 : 3; //
            path_.emplace_back(position.first, position.second - 1); //move right
            path_.emplace_back(position.first + 1, position.second); //move diagonal left
            return true;
        }
    }
    return false;
}

bool PathFinder::checkCloseRightDown(const std::pair<std::size_t, std::size_t> &position,
                                     const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.first < height_) && (position.second < width_)) {
        if (bitmap[position.first][position.second] &&
            (visited_[position.first][position.second] == visitationStatus)) {
            visited_[position.first][position.second] = (visitationStatus == 0) ? 1 : 3; //  /
            path_.emplace_back(position.first, position.second + 1); //move right
            path_.emplace_back(position.first + 1, position.second); //move diagonal left
            return true;
        }
    }
    return false;
}

bool PathFinder::checkFarLeftUp(const std::pair<std::size_t, std::size_t> &position,
                                const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.first > 0) && (position.second >= 2)) {
        ///some tricky idea, if we want to move to new cell we should on neighbours between, otherwise that's deadend
        if (!(bitmap[position.first - 1][position.second - 1] || bitmap[position.first][position.second - 1])) {
            return false;
        }
        ///
        if (bitmap[position.first - 1][position.second - 2] &&
            (visited_[position.first - 1][position.second - 2] == visitationStatus)) {
            visited_[position.first - 1][position.second - 2] = (visitationStatus == 0) ? 2 : 3;
            path_.emplace_back(position.first, position.second - 1);
            path_.emplace_back(position.first - 1, position.second - 2);
            return true;
        }
    }
    return false;
}

bool PathFinder::checkFarRightUp(const std::pair<std::size_t, std::size_t> &position,
                                 const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {

    if ((position.first > 0) && (position.second < width_ - 1)) {
        ///some tricky idea, if we want to move to new cell we should on neighbours between
        if (!(bitmap[position.first - 1][position.second] ||
              ((position.first < height_) ? bitmap[position.first][position.second] : false))) {
            return false;
        }
        ///
        if (bitmap[position.first - 1][position.second + 1] &&
            (visited_[position.first - 1][position.second + 1] == visitationStatus)) {
            visited_[position.first - 1][position.second + 1] = (visitationStatus == 0) ? 1 : 3; // /
            path_.emplace_back(position.first, position.second + 1);
            path_.emplace_back(position.first - 1, position.second + 2);
            return true;

        }
    }
    return false;

}

bool PathFinder::checkFarLeftDown(const std::pair<std::size_t, std::size_t> &position,
                                  const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.first < height_) && (position.second >= 2)) {
        ///some tricky idea, if we want to move to new cell we should on neighbours between
        if (!(((position.first > 0) ? (bitmap[position.first - 1][position.second - 1]) : false) ||
              bitmap[position.first][position.second - 1])) {
            return false;
        }
        ///
        if (bitmap[position.first][position.second - 2] &&
            (visited_[position.first][position.second - 2] == visitationStatus)) {
            visited_[position.first][position.second - 2] = (visitationStatus == 0) ? 1 : 3;
            path_.emplace_back(position.first, position.second - 1);
            path_.emplace_back(position.first + 1, position.second - 2);
            return true;
        }
    }
    return false;
}

bool PathFinder::checkFarRightDown(const std::pair<std::size_t, std::size_t> &position,
                                   const std::vector<std::vector<uint8_t >> &bitmap, uint8_t visitationStatus) {
    if ((position.first < height_) && (position.second < (width_ - 1))) {
        ///some tricky idea, if we want to move to new cell we should on neighbours between
        if (!(((position.first > 0) ? bitmap[position.first - 1][position.second] : false) ||
              bitmap[position.first][position.second])) {
            return false;
        }
        ///
        if (bitmap[position.first][position.second + 1] &&
            (visited_[position.first][position.second + 1] == visitationStatus)) {
            visited_[position.first][position.second + 1] = (visitationStatus == 0) ? 2 : 3; //
            path_.emplace_back(position.first, position.second + 1);
            path_.emplace_back(position.first + 1, position.second + 2);
            return true;
        }
    }
    return false;
}

bool PathFinder::isFullyCovered(const std::vector<std::vector<uint8_t>> &bitmap) const {
    for (size_t i = 0; i < bitmap.size(); i++) {
        for (size_t j = 0; j < bitmap[i].size(); j++) {
            if (bitmap[i][j] && (visited_[i][j] != 3)) {
                return false;
            }
        }
    }
    return true;
}
