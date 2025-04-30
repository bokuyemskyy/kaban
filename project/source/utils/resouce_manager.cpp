#include <algorithm>

#include "resource_manager.hpp"
#include "resources.hpp"

std::unordered_map<std::string, Resource> ResourceManager::resources;

Resource ResourceManager::getResource(std::string filename) {
    std::ranges::replace(filename, '.', '_');
    std::ranges::replace(filename, '-', '_');

    auto it = resources.find(filename);
    if (it == resources.end()) return {nullptr, 0};
    return it->second;
}

void ResourceManager::loadResources() {
    resources.emplace("p_png", Resource(p_png, p_png_len));
    resources.emplace("P_png", Resource(P_png, P_png_len));
    resources.emplace("n_png", Resource(n_png, n_png_len));
    resources.emplace("N_png", Resource(N_png, N_png_len));
    resources.emplace("b_png", Resource(b_png, b_png_len));
    resources.emplace("B_png", Resource(B_png, B_png_len));
    resources.emplace("r_png", Resource(r_png, r_png_len));
    resources.emplace("R_png", Resource(R_png, R_png_len));
    resources.emplace("q_png", Resource(q_png, q_png_len));
    resources.emplace("Q_png", Resource(Q_png, Q_png_len));
    resources.emplace("k_png", Resource(k_png, k_png_len));
    resources.emplace("K_png", Resource(K_png, K_png_len));
    resources.emplace("mark_png", Resource(mark_png, mark_png_len));
    resources.emplace("last_png", Resource(last_png, last_png_len));
    resources.emplace("round_png", Resource(round_png, round_png_len));
    resources.emplace("capture_mp3", Resource(capture_mp3, capture_mp3_len));
    resources.emplace("move_self_mp3", Resource(move_self_mp3, move_self_mp3_len));
}