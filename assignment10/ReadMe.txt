===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. main.py
----------------------------------------------------

PURPOSE
  Implements and trains VGG-16 and ResNet-50 models
  to perform image classification on the CIFAR-10
  dataset (50000 train, 10000 test).
  Loads a pretrained checkpoint, trains for 1 epoch
  using Adam optimizer with CrossEntropy loss, and
  evaluates test accuracy.

  The program proceeds in the following stages:
    Stage 1: Load and preprocess CIFAR-10 data
    Stage 2: Choose and load pretrained model
    Stage 3: Train for 1 epoch
    Stage 4: Evaluate on test set

RUN
  Run on Google Colab with GPU (T4 recommended):

    1. Mount Google Drive
    2. Upload .ckpt checkpoint files
    3. Define model classes (vgg16_full, resnet50_full)
    4. Run main.py cell

  Or run locally:

    python main.py

HOW TO ADJUST PARAMETERS
  To switch between VGG-16 and ResNet-50, find
  the "Choose model" section:

      # VGG-16
      model = vgg16().to(device)
      PATH = '/content/vgg16_epoch250.ckpt'

      # ResNet-50
      model = ResNet50_layer4().to(device)
      PATH = '/content/resnet50_epoch285.ckpt'

  Comment/uncomment the desired model pair.

  To adjust training parameters, find the
  "Hyper-parameters" section:

      num_epochs = 1
      learning_rate = 0.001

  - num_epochs
      Number of full passes over the training set.
      Default: 1

  - learning_rate
      Step size for Adam optimizer.
      Default: 0.001

  - batch_size
      Number of training samples per step.
      Default: 100

NOTE
  Requires pretrained checkpoint files:
    vgg16_epoch250.ckpt   (VGG-16,   ~85% test acc)
    resnet50_epoch285.ckpt (ResNet-50, ~82% test acc)
  Requires CIFAR-10 dataset (auto-downloaded via
  torchvision if download=True).
  Install required packages before running:
    pip install torch torchvision

----------------------------------------------------
2. vgg16_full.py
----------------------------------------------------

PURPOSE
  Defines the VGG class implementing VGG-16
  architecture with the following structure:

    input -> [Conv+ReLU] x2 -> MaxPool
          -> [Conv+ReLU] x2 -> MaxPool
          -> [Conv+ReLU] x3 -> MaxPool
          -> [Conv+ReLU] x3 -> MaxPool
          -> [Conv+ReLU] x3 -> MaxPool
          -> Dropout -> FC(512) -> BN -> ReLU
          -> Dropout -> FC(10)

  Provides two helper functions:
    make_layers() : Builds Conv+ReLU+MaxPool
                    sequence from cfg list
    vgg16()       : Returns VGG instance with
                    VGG-16 configuration

  Used by main.py as the VGG-16 classifier
  backend for CIFAR-10 classification.

NOTE
  Do not run this file directly.
  Import and use via main.py.
  Conv weights are initialized with He initialization
  (std = sqrt(2/n)).
  Bias vectors are initialized to zero.

----------------------------------------------------
3. resnet50_full.py
----------------------------------------------------

PURPOSE
  Defines the ResNet50_layer4 class implementing
  a modified ResNet-50 architecture with the
  following structure:

    input -> Conv(7x7) -> MaxPool
          -> Layer2: ResidualBlock x3
          -> Layer3: ResidualBlock x4
          -> Layer4: ResidualBlock x6
          -> AvgPool -> FC(10)

  Each ResidualBlock uses a bottleneck structure:
    1x1 conv -> 3x3 conv -> 1x1 conv + skip connection

  Provides two helper functions:
    conv1x1() : 1x1 Conv + BN + ReLU block
    conv3x3() : 3x3 Conv + BN + ReLU block

  Used by main.py as the ResNet-50 classifier
  backend for CIFAR-10 classification.

NOTE
  Do not run this file directly.
  Import and use via main.py.
  This architecture differs from the original
  ResNet-50 (adapted for CIFAR-10 32x32 input).
  Conv and Linear weights are initialized with
  Xavier uniform initialization.
  downsample=True halves the feature map size
  via stride=2 in the first 1x1 conv.

===================================================