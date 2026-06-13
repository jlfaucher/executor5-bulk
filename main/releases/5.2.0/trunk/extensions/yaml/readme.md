# Background

In March 2026 Josep Maria Blasco and Rony G. Flatscher experimented with
Claude's Max model to create an ooRexx class for processing YAML, comparable to
`json.cls`. YAML has become important in the context of Kubernetes and related
technologies, but also for GitHub Actions.

One speciality is the support for XML: ooRexx' YAML implementation allows
mapping YAML definitions to XML renderings, and create YAML renderings from XML
definitions. Therefore the DTD, XSD, and the XSL files to become able to process
the XML renderings get enclosed with `yaml.cls`.

## Installation's `bin` Directory

These are the files that should be deployed in installations:

- yaml.cls      ... `bin` directory or equivalent
- yaml.dtd      ... `bin` directory or equivalent
- yaml.xsd      ... `bin` directory or equivalent
- xmlToYaml.xsl ... `bin` directory or equivalent

## ooRexx Test Suite

This is the file that should go to the ooRexx test suite:

- yaml.testGroup           ... `test\trunk\ooRexx\extensions\yaml` directory
- test_all_constructs.yaml ... `test\trunk\ooRexx\extensions\yaml` directory


## Internal Quick Testing & Official YAML Test Suite Testing

The following files are for testing:

- test_yaml.rex
- test_all_constructs.yaml
- test_frontmatter.yaml

- test_yaml-test-suite.rex
- test_yaml-single.rex


---rgf, 2026-03-26, 2026-03-28
