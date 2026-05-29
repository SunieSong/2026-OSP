===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. two_layer_net.ipynb
----------------------------------------------------

PURPOSE
  Implements and trains a two-layer fully-connected
  neural network (Multi-Layer Perceptron) to perform
  image classification on the CIFAR-10 dataset.
  Loads and preprocesses CIFAR-10 data (19000 train,
  1000 validation, 1000 test), then validates the
  implementation via gradient check and trains the
  network using SGD with learning rate decay.
  Supports hyperparameter tuning over hidden layer
  size, learning rate, and regularization strength
  to maximize validation accuracy.

  The notebook proceeds in the following stages:
    Stage 1: Forward pass (scores and loss) verification
    Stage 2: Backward pass (gradient) verification
    Stage 3: Train on toy data (loss < 0.02)
    Stage 4: Load and preprocess CIFAR-10 data
    Stage 5: Train with default hyperparameters
    Stage 6: Tune hyperparameters, store best model
    Stage 7: Visualize weights of best network
    Stage 8: Evaluate best model on test set

RUN
  Open Jupyter Notebook and run all cells in order:

    jupyter notebook two_layer_net.ipynb

  Run each cell sequentially with Shift+Enter,
  or use Kernel -> Restart & Run All.

HOW TO ADJUST PARAMETERS
  To adjust default training parameters, find
  the "Train a network" cell:

      hidden_size = 50
      learning_rate = 1e-4
      learning_rate_decay = 0.95
      reg = 0.25
      num_iters = 1000
      batch_size = 200

  - hidden_size
      Number of neurons in the hidden layer.
      Larger value increases model capacity.
      Default: 50

  - learning_rate
      Step size for SGD parameter update.
      Larger value = faster but less stable learning.
      Default: 1e-4

  - learning_rate_decay
      Multiplicative factor applied to learning rate
      after each epoch.
      Default: 0.95

  - reg
      L2 regularization strength.
      Larger value penalizes large weights more strongly,
      reducing overfitting.
      Default: 0.25

  - num_iters
      Total number of SGD update steps.
      Default: 1000

  - batch_size
      Number of training samples per SGD step.
      Default: 200

  To adjust hyperparameter search range, find
  the "Tune your hyperparameters" cell:

      hidden_sizes = [100, 150]
      learning_rates = [1e-3, 3e-3]
      regs = [0.1, 0.5]

NOTE
  Requires classifier/neural_net.py to be present
  in the classifiers/ subdirectory.
  Requires CIFAR-10 dataset extracted to:
    datasets/cifar-10-batches-py/
  Install required packages before running:
    conda install numpy matplotlib scipy Pillow
    pip install imageio future
  All result plots are displayed inline within
  the notebook.

----------------------------------------------------
2. classifier/neural_net.py
----------------------------------------------------

PURPOSE
  Defines the TwoLayerNet class implementing a
  two-layer fully-connected neural network with
  the following architecture:

    input -> FC layer -> ReLU -> FC layer -> Softmax

  Provides three core methods:
    loss()    : Forward pass (scores, softmax loss,
                L2 regularization) and backward pass
                (gradients via backpropagation)
    train()   : SGD training loop with mini-batch
                sampling and learning rate decay
    predict() : Class prediction by argmax of scores

  Used by two_layer_net.ipynb as the classifier
  backend for CIFAR-10 classification.

NOTE
  Do not run this file directly.
  Import and use via two_layer_net.ipynb.
  Weight matrices W1, W2 are initialized with
  small random values (std=1e-4).
  Bias vectors b1, b2 are initialized to zero.

===================================================