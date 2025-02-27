#ifndef QLEARNING_H
#define QLEARNING_H

#include <vector>

class QLearning {
public:
  QLearning(int numStates, int numActions, float learningRate,
            float discountFactor, float explorationRate);

  int ChooseAction(int state);
  void UpdateQValue(int state, int action, float reward, int nextState);
  void UpdateEpsilon();

private:
  int numStates;
  int numActions;
  float alpha;   // Learning rate
  float gamma;   // Discount factor
  float epsilon; // Exploration rate
  std::vector<std::vector<float>> QTable;
  float epsilonDecayRate = 0.995;
  float minEpsilon = 0.1;

  int MaxAction(int state);
};

#endif // QLEARNING_H
