(function($) {

Drupal.behaviors.accountFieldsetSummaries = {
  attach: function (context, settings) {
    $('fieldset#edit-user', context).drupalSetSummary(function (context) {
      var name = $('#edit-name').val() || Drupal.settings.anonymous;

      return Drupal.t('Owned by @name', { '@name': name });
    });

    $('fieldset#edit-revision-information', context).drupalSetSummary(function (context) {
      var revisionCheckbox = $('#edit-revision', context);

      // Return 'New revision' if the 'Create new revision' checkbox is checked,
      // or if the revision log message is entered.
      if (revisionCheckbox.is(':checked') || $('#edit-log', context).val()) {
        return Drupal.t('New revision');
      }

      return Drupal.t('No revision');
    });
  }
};

})(jQuery);
