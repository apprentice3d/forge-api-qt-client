# Forge Qt SDK

Easily integrate Forge REST APIs into your C++ application.

## Overview

This [Qt](https://www.qt.io//) based SDK enables you to easily integrate
the Forge REST APIs into your C++ application
<!--, including <a
href="https://developer.autodesk.com/en/docs/oauth/v2/overview/"
target="_blank">OAuth</a>, <a
href="https://developer.autodesk.com/en/docs/data/v2/overview/"
target="_blank">Data Management</a>, <a
href="https://developer.autodesk.com/en/docs/model-derivative/v2/overview/"
target="_blank">Model Derivative</a>, and <a
href="https://developer.autodesk.com/en/docs/design-automation/v2/overview/"
target="_blank">Design Automation</a>.
-->
### Requirements

* Qt 5.6 and above.
* A registered app on the <a
  href="https://developer.autodesk.com/myapps" target="_blank">Forge
  Developer portal</a>.

### Installation

Copy the content of [src](./src) folder from this repo into your project
 and reference it accordingly.

## Tutorials

Each part of this SDK is presented with a README with appropriate
 tutorial content:

 - [Token acquiring](src/oauth)
 - [Bucket and object management](src/oss)
 - [Model derivative](src/model_derivative)
