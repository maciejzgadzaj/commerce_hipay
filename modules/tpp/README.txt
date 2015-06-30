
DESCRIPTION
===========

Commerce Hipay TPP submodule provides off-site payment method based on
Hipay TPP REST API (http://hipay-tpp-gateway-api.readthedocs.org/en/latest/).

* supports both full-page redirection as well as iframe integration
* supports authorization/capture/refund/cancel transactions
* supports antifraud features (CVC, AVS, 3-D Secure)
* additional request/response debugging


INSTALLATION
============

Hipay Fullservice API configuration
-----------------------------------

Go to https://merchant.hipay-tpp.com/ and configure the following settings:

1. Integration / Payment Procedure

   * Default payment procedure: select 'On-demand data capture (by the merchant)'

   * Default Electronic Commerce Indicator (ECI): select '7 - E-commerce with
     SSL encryption'. (This does not really matter though, as the module will
     send this value anyway in each relevant API call.)

2. Integration / Redirect Pages

   * Default Redirect Pages - provide any URL for Accept, Decline, Pending,
     Cancel and Exception Page. These do not matter, as the module will generate
     and send real values in each relevant API call.

   * Feedback Parameters: enable 'I want to receive transaction feedback
     parameters on the redirect pages'

3. Integration / Security Settings

   * Data Verification: provide 'Secret Passphrase' - this has to have the same
     value as the 'Secret Passphrase' in the payment method configuration in
     Drupal.

   * IP Restriction: having this enabled is highly recommended. Add IP addresses
     of all your sites calling the Hipay TPP API.

   * Api credentials: generate new credentials if they haven't been already
     generated for you, then add them in your payment method configuration
     in Drupal in 'API username' and 'API password' fields.

4. Integration / Notifications

   * Server-to-Server Notification: these notifications are required for Drupal
     payment transactions to be updated to successful states. Provide URL as
     https://your-site.com/commerce-hipay-tpp/notify
     In 'I want to be notified for the following transaction statuses'
     select 'All'.

   * Integration / E-mail Communications

     Configure as per your specific requirements.


Drupal payment method configuration
-----------------------------------

1. Enable the Commerce Hipay TPP module.

2. Enable and configure the Hipay TPP payment method.

   Enable the Hipay TPP payment method on your store's Payment methods page
   (admin/commerce/config/payment-methods).

   Next go to the payment method configuration page, select required
   environment, enter relevant API username and password values (as described
   above), and configure other options as required.



TEST CARD NUMBERS
=================

When testing you may use the following test card numbers with any future
expiry date.

| Card plan                   | Card number       |
|-----------------------------|-------------------|
| Visa                        | 4111111111111111  |
| MasterCard                  | 5399999999999999  |
| Visa 3-D Secure             | 4000000000000002  |
| MasterCard / Visa "refused" | 4111113333333333  |



LINKS
=====

* Hipay Fullservice merchant account
  https://merchant.hipay-tpp.com/

* Hipay Fullservice Support Center
  http://help.hipay.com/tpp

* HiPay TPP Gateway API documentation
  http://hipay-tpp-gateway-api.readthedocs.org/en/latest/
