<?php
/**
 * @file
 * Islandora PlotIt Solr results template.
 *
 * Variables available:
 * - $json_callback_url: The Solr search URL for use in the preprocess.
 *
 * @see template_preprocess_islandora_plotit_display_ploitit()
 */
?>


<div id="loading_overlay">
   <p>
      Loading <span></span>
   </p>
</div>

<section id="sidebar">
   <header>
      Plot-It
   </header>

   <section>
      <div id="filters">
         <text_filter params="label: '', placeholder: 'eg. Rocky Mountains'"></text_filter>

         <date_filter params="label: 'Date Range'"></date_filter>

         <checklist_filter params="label: 'Collection', field: 'group'"></checklist_filter>
         <checklist_filter params="label: 'Event Type', field: 'eventType'"></checklist_filter>
      </div>

      <hr />

      <div class="save-reset">
         <filter_save_results></filter_save_results>

         <filter_reset params="label: 'Reset All', filterGroupId: 'filters'"></filter_reset>
      </div>
   </section>

   <section>
      <header>How to use search</header>
      <p>
         Search below by choosing a facet or typing keywords. To restart your search, click on "Reset all Filters"
         above the map.
      </p>

      <p>
         <a class="help" href="http://cwrc.ca/Plot-It_Documentation" target="_blank">Help...</a>
      </p>
   </section>

   <section>
      <header>
         Project team members
      </header>
      <p>
         Jeffery Antoniuk, Brittney Broder, Susan Brown, Michael Brundin, Lisa Dublin, Isobel Grundy, Kathryn
         Holland, Mihaela Ilovan, Hamman Samuel, <a href="http://tenjin.ca" target="_blank">Tenjin</a>, Kristina
         Vyskocil, and Willow White
      </p>
   </section>
</section>

<main class="display">
   <expander params="expandedText: 'Hide Timeline', collapsedText: 'Show Timeline'">
      <timeline></timeline>
   </expander>

   <spotlight></spotlight>

   <expander>
      <tab_pane>
         <atlas data-tab-label="Map View" params=" center: '38.479394673276445, -115.361328125',
                                                 zoom: 1,
                                                 colorKey: 'eventType',
                                                 colors: {Bibliography: '#00f', Publication: '#0f0', Unknown: 'grey'},
                                                 opacity: '0.5',
                                                 markerWidth: 18,
                                                 markerHeight: 18">
         </atlas>
         <style>
            grid .grid-start,
            grid .grid-end {
               white-space: nowrap;
            }
            grid .grid-lat-lng {
               max-width: 12em;
               overflow: auto;
            }
         </style>
         <grid data-tab-label="Grid View" params="columns: {
                                                     'Event': 'label',
                                                     'Collection': 'group',
                                                     'Location': 'location',
                                                     'Lat/Lng': 'latLng',
                                                     'Start': 'startDate',
                                                     'End': 'endDate'},
                                                  initialSortBy: ['group', 'label-za']">
         </grid>
      </tab_pane>
   </expander>
</main>


<!--
===============================================
               Component Templates
===============================================
-->
<div id="templates" style="display:none;">

   <!--
   ===============================================
                      Spotlight
   ===============================================
   -->
   <template id="spotlight-template">
      <!-- ko if: lightboxVisible -->
      <div class="spotlight-lightbox" data-bind="click: function(){ toggleLightbox() }">
         <div>
            <img data-bind="src: selected().images" />
         </div>
      </div>
      <!-- /ko -->
      <section data-bind="if: selected() == null, visible: selected() == null">
         <p class="placeholder">
            Click an event in the timeline or map to see details
         </p>
      </section>
      <section data-bind="if: selected() != null, visible: selected() != null">
         <img data-bind="visible: selected().images, src: selected().images, click: function(){ toggleLightbox() }" />
         <iframe allowfullscreen=true
                 data-bind="visible: selected().videos, src: selected().videos">
         </iframe>
         <div>
            <header>
                    <span data-bind="if: selected().startDate">
                        <span data-bind="html: selected().startDate"></span>
                        <span data-bind="if: selected().endDate">
                            to
                            <span data-bind="text: selected().endDate"></span>
                        </span>
                    </span>
                    <span data-bind="if: selected().location">
                        <span data-bind="html: selected().location"></span>
                    </span>

                    <span data-bind="if: selected().link">
                      <a target="_blank" data-bind="html: 'link' , attr: { href: selected().link}"></a>
                    </span>
            </header>
            <section>
               <header>
                  <span data-bind="html: selected().longLabel"></span>
               </header>
               <span data-bind="html: selected().description"></span>

               <p data-bind="if: selected().urls">
                  <a target="_blank" data-bind="href: selected().urls">More...</a>
               </p>

               <!-- ko if: selected().citations -->
               <div>Sources:</div>
               <p data-bind="html: selected().citations">
               </p>
               <!-- /ko -->

               <p data-bind="if: selected().source">
                  <a target="_blank" data-bind="href: selected().source">(Source)</a>
               </p>
            </section>
         </div>
      </section>
   </template>

   <!--
   ===============================================
                     Timeline
   ===============================================
   -->
   <template id="timeline-template">
      <section>
         <div class="unplottable">
            <span data-bind="text: unplottableCount"></span>
            of
            <span data-bind="text: CWRC.rawData().length"></span>
            lack time data
         </div>
      </section>
      <section class="timeline-ruler">
         <div data-bind="foreach: ruler.step(ruler.shiftUnit(ruler.unit(), 1))">
            <span data-bind="text: $data.label, style: {left: $data.position}"></span>
         </div>
         <div data-bind="foreach: ruler.step(ruler.unit())">
            <span data-bind="text: $data.label, style: {left: $data.position}"></span>
         </div>
      </section>
      <resizer>
         <div data-bind="event: { mousedown: $parent.mouseHandler.onDragStart,
                                  touchstart: $parent.touchHandler.onTouch }">
            <section class="timeline-viewport" data-bind="event: {wheel: $parent.mouseHandler.onScroll}">
               <div class="canvas"
                    style="position: relative"
                    data-bind="style: {
                                         width: $parent.canvas.bounds.width(),
                                         height: $parent.canvas.bounds.height(),
                                         msTransform: $parent.viewport.translateTransform,
                                         webkitTransform: $parent.viewport.translateTransform,
                                         transform: $parent.viewport.translateTransform
                                       },
                               foreach: {data: $parent.canvas.tokens, as: 'token'} ">
                  <div class="event" data-bind="visible: token.visible,
                                                css: { selected: token.isSelected(),
                                                       period: token.data.endDate
                                                     },
                                                style: {
                                                         left: 0,
                                                         height: token.height() + 'px',
                                                         width: token.lineWidth() + 'px',
                                                         zIndex: token.layer(),
                                                         msTransform: token.markerTransform,
                                                         webkitTransform: token.markerTransform,
                                                         transform: token.markerTransform
                                                       },
                                                attr: {id: 'token-' + token.id}">
                     <a href="#" data-bind="event: {
                                                    mouseup: $parents[1].mouseHandler.recordMouseUp,
                                                    mousedown: $parents[1].mouseHandler.recordMouseDown,
                                                    mouseover: function(){ token.isHovered(true) },
                                                    mouseout:  function(){ token.isHovered(false) }
                                                    },
                                            style: {
                                                width: token.labelWidth,
                                                maxWidth: token.maxLabelWidth
                                            }">
                        <span data-bind="html: token.maxLabelWidth() > 20 ? token.data.label : '...'"></span>
                     </a>
                  </div>
               </div>
            </section>
         </div>
      </resizer>
   </template>


   <!--
   ===============================================
                        Grid
   ===============================================
   -->
   <template id="grid-template">
      <div class="grid-controls">
         <div class="grid-sorter">
            <p class="prompt">Sort <span data-bind="visible: sortContexts().length > 0">by</span></p>
            <!-- Building a "widget" for editing each sorting context-->
            <div data-bind="foreach: sortContexts">
               <div>
                  <select
                        data-bind="options: $parent.allContexts,
                                optionsText: 'displayName',
                                optionsValue: 'name',
                                optionsCaption:'Choose...',
                                value: $data.name"></select>
                  <a href="#"
                     data-bind="click: function() { $data.reverse() },
                             text: $data.getFieldDirectionLabel()"></a>
                  <a href="#" data-bind="click: function() { $parent.removeSortBy($data) }">x</a>
               </div>
            </div>

            <a href="#" title="Add Sorting Rule" data-bind="click: function(){addContext('')} ">by...</a>
         </div>

         <div class="grid-column-chooser">
            <input type="button" data-bind="click: toggleColumnSelector" value="Columns..." />

            <div data-bind="visible: columnSelectVisible,foreach: Object.keys(columns)">
               <label>
                  <input type="checkbox" data-bind="value: $data, checked: $parent.visibleColumns">
                  <span data-bind="text: $data"></span>
               </label>
            </div>
         </div>
         <!-- ko if: columnSelectVisible-->
         <div class="grid-column-chooser-area" data-bind=" click: toggleColumnSelector">
         </div>
         <!-- /ko -->
      </div>

      <table>
         <!-- explicit thead & tbody are important, otherwise the browser assumes incorrect things-->
         <thead>
         <tr data-bind="foreach: Object.keys(columns)">
            <th data-bind="visible: $parent.isColumnVisible($data)">
               <a href="#" data-bind="click: function(){ $parent.sortByColumn($data) }">
                  <span data-bind="text: $data"></span>
                  <span data-bind="text: $parent.getArrow($data)"></span>
               </a>
            </th>
         </tr>
         </thead>
         <tbody data-bind="foreach: {data: itemsOnCurrentPage, as: 'item'}">
         <tr data-bind="click: function(){ CWRC.selected(item)}">
            <!-- ko foreach: {data: Object.keys($parent.columns), as: 'columnLabel'} -->
            <td data-bind="html: $parents[1].getData(item, columnLabel) || 'n/a',
                           css: $parents[1].getColumnClass(columnLabel),
                           visible: $parents[1].isColumnVisible(columnLabel)">
            </td>
            <!-- /ko -->
            <td>
               <a target="_blank" data-bind="attr: {href: item['link']}">
                  Link
               </a>
            </td>
         </tr>
         </tbody>
      </table>

      <section class="pageControls" data-bind="visible: maxPageIndex() > 1">
         <span data-bind="visible: isFarAway(1)">
            <a href="#" data-bind="click: function(){ setPage(1) },
                                   attr: {selected: currentPageIndex() == 1}">1</a>
            <span data-bind="visible: isFarAway(2)">
              ...
            </span>
         </span>
         <span data-bind="foreach: pageNeighbourhood">
             <a href="#" data-bind="text: $data,
                                    click: function(){ $parent.setPage($data) },
                                    attr: {selected: $parent.currentPageIndex() == $data}"></a>
         </span>
         <span data-bind="visible: isFarAway(maxPageIndex())">
             <span data-bind="visible: isFarAway(maxPageIndex() - 1)">
                 ...
             </span>
             <a href="#"
                data-bind="text: maxPageIndex,
                           click: function(){ setPage(maxPageIndex()) },
                           attr: {selected: currentPageIndex() == maxPageIndex()}"></a>
         </span>

         <div>
            <span>Jump To...</span>
            <select data-bind="value: currentPageIndex, options: pages"></select>
         </div>
      </section>
   </template>


   <!--
   ===============================================
                       Atlas
   ===============================================
   -->
   <template id="atlas-template">
      <section>
         <div class="unplottable">
            <span data-bind="text: unplottableCount"></span>
            of
            <span data-bind="text: CWRC.rawData().length"></span>
            lack map data
         </div>
      </section>
      <div id="historicalMapControls" title="Click to toggle the historical map">
         <label>
            <input type="checkbox" data-bind="checked: showHistoricalMap">
            <span>Historical Map</span>
         </label>

         <div data-bind="visible: showHistoricalMap">
            <label>
               <span>Map</span>
               <select data-bind="options: overlays,
                                  optionsText: getOverlayName,
                                  value: selectedOverlay"></select>
            </label>
            <label>
               <span>Opacity</span>
               <input id="historicalMapOpacity" type="range" min="0.0" max="1.0" step="0.05"
                      data-bind="value: historicalMapOpacity" />
            </label>
         </div>
      </div>
      <!-- identifying by ID does limit to one map per page, but that works for now -->
      <div data-bind="style: {height: canvasHeight}" id="map_canvas">
      </div>
      <resizer params="resizerObservable: canvasHeight, resizedId: 'map_canvas'"></resizer>
      <section class="legend" data-bind="visible: colorTable.hasMapping()">
         <header>Legend</header>
         <!-- ko foreach: colorTable.getLegendPairs() -->
         <div>
            <img data-bind="src: $data.icon"></span>
            <span data-bind="text: $data.name"></span>
         </div>
         <!-- /ko -->
      </section>
   </template>

   <!--
   ===============================================
                     Tab Pane
   ===============================================
   -->
   <template id="tab-pane-template">
      <header>
         <form data-bind="foreach: views">
            <label>
               <input type="radio" name="tabset" data-bind="checked: $parent.currentView,
                                                             value: $index()" />
               <span data-bind="text: $parent.tabName($data, $index())"></span>
            </label>
         </form>
      </header>
      <div data-bind="foreach: views">
         <div data-bind="visible: $parent.isView($index()), template: {nodes: [$data]}">
         </div>
      </div>
   </template>

   <!--
   ===============================================
                    Error Status
   ===============================================
   -->
   <template id="status-template">
      <div data-bind="visible: hasMessages()">
         <section data-bind="visible: notices().length > 0">
            <ul>
               <!-- ko foreach: notices() -->
               <li>
                  <span data-bind="text: $data"></span>
                  <a href="#" data-bind="visible: $parent.dismissable,
                                                           click: function(){ $parent.notices.splice($index(), 1)}">X</a>
               </li>
               <!-- /ko -->
            </ul>
         </section>
         <section data-bind="visible: warnings().length > 0">
            <header>Warning</header>
            <p data-bind="text: warningFlavour(), visible: warningFlavour()"></p>
            <ul>
               <!-- ko foreach: warnings() -->
               <li>
                  <span data-bind="text: $data"></span>
                  <a href="#" data-bind="visible: $parent.dismissable,
                                                           click: function(){ $parent.warnings.splice($index(), 1)}">X</a>
               </li>
               <!-- /ko -->
            </ul>
         </section>
         <section data-bind="visible: errors().length > 0">
            <ul>
               <!-- ko foreach: errors() -->
               <li>
                  <span data-bind="text: $data"></span>
                  <a href="#" data-bind="visible: $parent.dismissable,
                                                           click: function(){ $parent.errors.splice($index(), 1)}">X</a>
               </li>
               <!-- /ko -->
            </ul>
         </section>
      </div>
   </template>
</div>
