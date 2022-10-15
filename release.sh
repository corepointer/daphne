#!/usr/bin/env bash
#!/usr/bin/env bash
# Copyright 2022 The DAPHNE Consortium
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Stop immediately if any command fails.
set -e

function exit_with_usage {
  cat << EOF
usage: $0 --version VERSION --githash GIT_HASH [ --gpgkey GPG_KEY ] [ --artifact PATH_TO_FILE ]

--gpgkey: The key ID to use for signing. If supplied, an attempt to sign the artifact will be made.
          Consider setting GNUPGHOME to point to your GPG keyring.

--artifact: If supplied, building the release artifact will be skipped and the script will only perform
            checksumming and optional signing.
EOF
  exit 1
}

if [ $# -eq 0 ]; then
  exit_with_usage
fi

DAPHNE_VERSION=-1
GIT_HASH=0
BUILD=1
GPG_KEY=0
ARTIFACT_PATH=""

while [[ $# -gt 0 ]]; do
    key=$1
    shift
    case $key in
        -h|--help)
            exit_with_usage
            ;;
        --version)
            DAPHNE_VERSION=$1
            shift
            ;;
        --githash)
            GIT_HASH=$1
            shift
            ;;
        --gpgkey)
            GPG_KEY=$1
            shift
            ;;
        --artifact)
            ARTIFACT_PATH=$1
            BUILD=0
            shift
            ;;
        *)
            unknown_options="${unknown_options} ${key}"
            exit_with_usage
            ;;
    esac
done

if [ $BUILD -eq 1 ]; then
  if [[ "$DAPHNE_VERSION" == "-1" ]] || [[ $GIT_HASH == "0" ]]; then
    echo Error: version or hash not supplied
    exit_with_usage
  fi

  echo "Building Daphne $DAPHNE_VERSION release from git commit $GIT_HASH"

  # set the requested commit to build Daphne
  git checkout "$GIT_HASH"
  source pack.sh --version "$DAPHNE_VERSION"
  # return to the previous branch
  git checkout -

  cd "$daphneBuildDir"
  sha512sum "$PACK_ROOT".tgz > "$PACK_ROOT.tgz.sha512sum"
  sha512sum -c "$PACK_ROOT".tgz.sha512sum
  echo
  cd - > /dev/null

  echo "git tag $GIT_HASH $DAPHNE_VERSION"

  echo "git push $DAPHNE_REPO_URL"

  if ! [[ "$GPG_KEY" -eq "0" ]]; then
    gpg --sign "$PACK_ROOT".tgz
  else
    echo "No GPG Key given - don't forget to sign the artifact manually"
  fi
else
  sha512sum "$ARTIFACT_PATH" > "$ARTIFACT_PATH.sha512sum"
  sha512sum -c "$ARTIFACT_PATH.sha512sum"
  echo

  echo "$GPG_KEY"

  if [ "$GPG_KEY" != "0" ]; then
    gpg --detach-sign --armor --default-key "$GPG_KEY" "$ARTIFACT_PATH"
  else
    echo "No GPG Key given - don't forget to sign the artifact manually"
  fi
fi

set +e
