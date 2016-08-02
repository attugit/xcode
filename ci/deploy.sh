#!/bin/bash

chown root:root -R /logs &&
chown root:root -R /root/.ssh &&
chmod 600 -R /root/.ssh &&
git config --global user.name "travis-ci" &&
git config --global user.email "bot-attugit.xcode@travis-ci.com" &&
git clone git@github.com:attugit/xcode.git --branch=gh-pages &&
mv /logs/latest.log ./xcode/index.md &&
pushd ./xcode &&
git add ./index.md &&
git commit --allow-empty -m "latest log" &&
git push -u origin gh-pages
