#include "include/QLearning.h"
#include <algorithm> // For std::max_element

QLearning::QLearning(int numStates, int numActions, float learningRate,
                     float discountFactor, float explorationRate)
    : numStates(numStates), numActions(numActions), alpha(learningRate),
      gamma(discountFactor), epsilon(explorationRate) {
  QTable.resize(numStates, std::vector<float>(numActions, 0));
}

// Inside QLearning class:
void QLearning::UpdateEpsilon() {
  // Decay epsilon over time, reducing it to a minimum value
  if (epsilon > minEpsilon) {
    epsilon *= epsilonDecayRate; // Decay rate < 1, e.g., 0.995
  }
}

int QLearning::ChooseAction(int state) {
  if (static_cast<float>(rand()) / RAND_MAX < epsilon) {
    return rand() % numActions; // Exploration
  } else {
    return MaxAction(state); // Exploitation
  }
}

void QLearning::UpdateQValue(int state, int action, float reward,
                             int nextState) {
  float maxNextQ =
      *std::max_element(QTable[nextState].begin(), QTable[nextState].end());
  QTable[state][action] =
      QTable[state][action] +
      alpha * (reward + gamma * maxNextQ - QTable[state][action]);
}

int QLearning::MaxAction(int state) {
  return std::distance(
      QTable[state].begin(),
      std::max_element(QTable[state].begin(), QTable[state].end()));
}
