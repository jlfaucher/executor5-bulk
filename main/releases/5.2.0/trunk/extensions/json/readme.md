# Background

In March/April 2026 Josep Maria Blasco and Rony G. Flatscher experimented with
Claude's Max model to update and improve `json.cls`.

One new speciality is the support for XML: the implementation now allows
mapping JSON definitions to XML renderings, and create JSON renderings from XML
definitions. Therefore the DTD, XSD, and the XSL files to become able to process
the XML renderings get enclosed with `json.cls`.

## Installation's `bin` Directory

These are the files that should be deployed in installations:

- json.cls      ... `bin` directory or equivalent
- json.dtd      ... `bin` directory or equivalent
- json.xsd      ... `bin` directory or equivalent
- xmlToJson.xsl ... `bin` directory or equivalent

## ooRexx Test Suite

These are the files that should go to the ooRexx test suite:

- json_02.testGroup        ... `test\trunk\ooRexx\extensions\yaml` directory
- test_all_constructs.json ... `test\trunk\ooRexx\extensions\yaml` directory


## Internal Quick Testing & JSON Test Suite Testing

The following files are for testing:

- test_json.rex
- test_all_constructs.json
- test_json_seriot.rex

- needs PATH set to ooRexxUnit:
  - run_json_testGroup.rex
  - json_01_Claude.testGroup

---rgf, 2026-04-05
