===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. main.py
----------------------------------------------------

PURPOSE
  Implements and trains UNet and ResNet-encoder-UNet
  models to perform semantic segmentation on the
  Pascal VOC 2012 dataset (train/val split 80/20).
  Loads a pretrained checkpoint, trains for 1 epoch
  using Adam optimizer with CrossEntropy loss, and
  evaluates val accuracy.

  The program proceeds in the following stages:
    Stage 1: Load and preprocess Pascal VOC 2012 data
    Stage 2: Choose and load pretrained model
    Stage 3: Train for 1 epoch
    Stage 4: Evaluate on validation set and save
             segmentation result images

RUN
  Run on Google Colab with GPU (T4 recommended):

    1. Mount Google Drive
    2. Upload pretrained .pth checkpoint files
    3. Define model classes and module functions
    4. Run main.py cell

  Or run locally:

    python main.py

HOW TO ADJUST PARAMETERS
  To switch between UNet and ResNet-encoder-UNet,
  find the "Initialize the model" section:

      # UNet
      model = Unet(in_channels=3, out_channels=22)
      pretrained_path = 'UNet_trained_model.pth'

      # ResNet-encoder-UNet
      model = UNetWithResnet50Encoder(n_classes=22)
      pretrained_path = 'resnet_encoder_unet.pth'

  Comment/uncomment the desired model pair.

  To adjust training parameters, find the
  "Hyper-parameters" section:

      batch_size = 16
      learning_rate = 0.001
      epochs = 40

  - batch_size
      Number of training samples per step.
      Default: 16

  - learning_rate
      Step size for Adam optimizer.
      Default: 0.001

  - epochs
      Number of full passes over the training set.
      Automatically set to 1 when pretrained
      checkpoint is loaded.

  - resize_size
      Input image resize resolution.
      Default: 256

NOTE
  Requires pretrained checkpoint files:
    UNet_trained_model.pth        (UNet)
    resnet_encoder_unet.pth       (ResNet-encoder-UNet)
  Requires Pascal VOC 2012 dataset:
    Download from http://host.robots.ox.ac.uk/pascal/VOC/voc2012/
    Place under VOCdevkit/VOC2012/
  Tip: If data processing takes too long,
       reduce dataset size in datasets.py:
         self.imgnames = self.lines[:20]

----------------------------------------------------
2. modules.py
----------------------------------------------------

PURPOSE
  Defines train and validation functions used
  by main.py for model training and evaluation.

  Provides the following functions:
    train_model()             : Trains model for
                                one epoch using
                                forward pass,
                                loss computation,
                                backpropagation,
                                and optimizer step
    get_loss_train()          : Evaluates train
                                loss and accuracy
                                without gradient
                                computation
    val_model()               : Evaluates val loss
                                and accuracy, and
                                saves segmentation
                                result images
    accuracy_check()          : Computes pixel-wise
                                accuracy for a
                                single sample
    accuracy_check_for_batch(): Computes average
                                accuracy over a
                                batch

NOTE
  Do not run this file directly.
  Import and use via main.py.
  val_model() saves label and result images
  as PNG files to the specified directory.

----------------------------------------------------
3. UNet.py
----------------------------------------------------

PURPOSE
  Defines the Unet class implementing the original
  U-Net architecture for semantic segmentation
  with the following structure:

    Encoder (Contracting path):
      input -> conv(64) -> MaxPool
            -> conv(128) -> MaxPool
            -> conv(256) -> MaxPool
            -> conv(512) -> MaxPool
            -> conv(1024)

    Decoder (Expanding path):
      Upsample -> cat(conv4,1024) -> conv(512)
      Upsample -> cat(conv3,512)  -> conv(256)
      Upsample -> cat(conv2,256)  -> conv(128)
      Upsample -> cat(conv1,128)  -> conv(64)
      -> Conv1x1(22)

  Each conv block consists of:
    Conv(3x3) -> BN -> ReLU -> Conv(3x3) -> BN -> ReLU

  Output: segmentation map with 22 classes
  (20 VOC classes + background + edge)

HOW TO ADJUST PARAMETERS
  To change input/output channels, modify
  Unet initialization in main.py:

      model = Unet(in_channels=3, out_channels=22)

  - in_channels : Number of input image channels
                  Default: 3 (RGB)
  - out_channels: Number of segmentation classes
                  Default: 22

NOTE
  Do not run this file directly.
  Import and use via main.py.
  Skip connections concatenate encoder feature
  maps with decoder feature maps at each level.
  Bilinear upsampling is used in the decoder.

----------------------------------------------------
4. resnet_encoder_unet.py
----------------------------------------------------

PURPOSE
  Defines the UNetWithResnet50Encoder class
  implementing a U-Net with ResNet-50 encoder
  for semantic segmentation with the following
  structure:

    Encoder (ResNet-50 based):
      Layer1: Conv(7x7, stride=2) -> MaxPool
      Layer2: ResidualBlock x2 + ResidualBlock
              (downsample) -> output channel: 256
      Layer3: ResidualBlock x3 + ResidualBlock
              (downsample) -> output channel: 512

    Bridge:
      conv(512, 512)

    Decoder (Expanding path):
      UpConv1 -> cat(out2, 256+256) -> conv(256)
      UpConv2 -> cat(out1, 64+64)   -> conv(64)
      Upsample x2 -> Conv1x1(22)

  Each ResidualBlock uses a bottleneck structure:
    1x1 conv -> 3x3 conv -> 1x1 conv + skip connection

  Provides two helper functions:
    conv1x1() : 1x1 Conv + BN block
    conv3x3() : 3x3 Conv + BN block

HOW TO ADJUST PARAMETERS
  To change number of output classes, modify
  model initialization in main.py:

      model = UNetWithResnet50Encoder(n_classes=22)

  - n_classes: Number of segmentation classes
               Default: 22

NOTE
  Do not run this file directly.
  Import and use via main.py.
  In Layer2 and Layer3, feature map size is
  halved using strided convolution (stride=2)
  at the last ResidualBlock.
  downsample=True applies stride=2 to reduce
  spatial resolution and match channel dimensions
  via 1x1 convolution on the skip connection.

===================================================